
#ifndef __CKFlywheelSpeedController__
#define __CKFlywheelSpeedController__

#pragma systemFile



#include <CKVexMotors.h>
#include <CKMovingAverage.h>




struct FlywheelSpeedController {
	IMEMotorSet flywheelMotors;

	MovingAverage maFlywheelSpeed;
	MovingAverage maSpeedError;

	// Characteristic model parameters
	// power = A e^( B speed )
	float A, B;

	// Quadradic, Proportional, Integral, & Derivative coefficients
	float Kq, Kp, Ki, Kd;

	float targetSpeed;
	float speedError;
	float controlPower;

	tSensors powerExpanderVoltagePort;
	float mainBatteryProportion;
};




void FlywheelSpeedControllerInit( FlywheelSpeedController& self ){
	MovingAverageInit( self.maFlywheelSpeed, 4 );
	MovingAverageInit( self.maSpeedError, 8 );

	self.Kq = 0;
	self.Kp = 0;
	self.Ki = 0;
	self.Kd = 0;

	self.A = 1;
	self.B = 1;

	self.powerExpanderVoltagePort = NoPort;
	self.mainBatteryProportion = 1.0;
}




void FlywheelSpeedControllerInit(
			FlywheelSpeedController& self,
			tMotor* ports, int nMotors,
			Motor393GearBox gearbox = M393Standard
){
	FlywheelSpeedControllerInit(self);

	IMEMotorSetInit( self.flywheelMotors, ports, nMotors, gearbox );
}





void FlywheelSpeedControllerInit(
			FlywheelSpeedController& self,
			float a, float b,
			tMotor* ports, int nMotors,
			Motor393GearBox gearbox = M393Standard
){
	FlywheelSpeedControllerInit( self, ports, nMotors, gearbox );

	self.A = a;
	self.B = b;
}




void FlywheelSpeedControllerInit(
			FlywheelSpeedController& self,
			float q, float p, float i, float d,
			float a, float b,
			tMotor* ports, int nMotors,
			Motor393GearBox gearbox = M393Standard
){
	FlywheelSpeedControllerInit( self, a, b, ports, nMotors, gearbox );

	self.Kq = q;
	self.Kp = p;
	self.Ki = i;
	self.Kd = d;
}




// LEGACY to not break the home boyz' code
void FlywheelSpeedControllerInit(
			FlywheelSpeedController& self,
			float q, float i, float d,
			float a, float b,
			tMotor* ports, int nMotors,
			Motor393GearBox gearbox = M393Standard
){
	FlywheelSpeedControllerInit( self, a, b, q, 0, i, d, ports, nMotors, gearbox );
}




void setMotorPortOfIME( FlywheelSpeedController& self, tMotor port ){
	setMotorPortOfIME( self.flywheelMotors, port );
}




void setCharacteristics( FlywheelSpeedController& self, float a, float b ){
	self.A = a;
	self.B = b;
}





void setGearbox( FlywheelSpeedController& self, Motor393GearBox gearbox ){
	setGearbox( self.flywheelMotors, gearbox );
}





void setFlywheelBatteryConfig( FlywheelSpeedController& self, tSensors vPort, float battProp ){
	self.powerExpanderVoltagePort = vPort;
	self.mainBatteryProportion = battProp;
}




float flywheelBatteryVoltage( FlywheelSpeedController& self ){
	float vMain = MainBatteryVoltage();
	tSensors vPort = self.powerExpanderVoltagePort;
	if( vPort == NoPort )
		return vMain;
	float vProport = self.mainBatteryProportion;
	return vProport * vMain + (1 - vProport) * powerExpanderVoltage(vPort);
}




float cruisingPower( FlywheelSpeedController& self ){
	if( self.targetSpeed < 1 ) return 0;
	return (self.A * exp( self.B * self.targetSpeed )) / flywheelBatteryVoltage(self);
}




void setPower( FlywheelSpeedController& self, float power ){
	// Never let the power go reverse (negative)
	self.controlPower = power;
	setPower( self.flywheelMotors, bound(self.controlPower, 0, 1) );
}




void setTargetSpeed( FlywheelSpeedController& self, float speed ){
	// No reverse speeds allowed.    For now. . .
	self.targetSpeed = maximum( 0, speed );
}




float getMeasuredSpeed( FlywheelSpeedController& self ){
	return getAverage( self.maFlywheelSpeed );
}




float getMeasuredAcceleration( FlywheelSpeedController& self ){
	return self.flywheelMotors.ime.acceleration;
}




void measure( FlywheelSpeedController& self ){
	// Measure the instantaneous motor speed, and update the average speed
	measure( self.flywheelMotors );
	nextSample( self.maFlywheelSpeed, absolute(self.flywheelMotors.ime.velocity) );
}




void update( FlywheelSpeedController& self ){
	measure( self );
	float measuredSpeed = getMeasuredSpeed(self);

	float previousError = self.speedError;
	self.speedError = measuredSpeed - self.targetSpeed;

	nextSample( self.maSpeedError, self.speedError );
	float eSquared = self.speedError * self.speedError;
	float dt = self.flywheelMotors.ime.dt;
	if( dt == 0 ) dt = 1;

	float qTerm = sgn(self.speedError)*eSquared;
	float pTerm = self.speedError;
	float iTerm = self.maSpeedError.sum;
	float dTerm = (self.speedError - previousError) / dt;

	setPower( self, cruisingPower(self) - (self.Kq*qTerm + self.Kp*pTerm + self.Ki*iTerm + self.Kd*dTerm) );
}




#endif
