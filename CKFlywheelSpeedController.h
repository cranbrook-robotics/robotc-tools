
#ifndef __CKFlywheelSpeedController__
#define __CKFlywheelSpeedController__

#pragma systemFile



#include <CKVexMotors.h>
#include <CKMovingAverage.h>



struct FlywheelSpeedController {
	IMEMotorSet flywheelMotors;

	MovingAverage maFlywheelSpeed;
	MovingAverage maSpeedError;

	float targetSpeed;
	float speedError;

	// power = A e^( B speed )
	float A, B;

	// Quadradic, Integral, & Derivative coefficients
	float Kq, Ki, Kd;

	float controlPower;
};


//MovingAverage maBattery;


void FlywheelSpeedControllerInit
(
			FlywheelSpeedController& self,
			float q, float i, float d, float a, float b,
			tMotor* ports,
			int nMotors,
			Motor393GearBox gearbox = M393Standard
)
{
	for(int i = 0 ; i < (int)nMotors; ++i)writeDebugStreamLine("%d", ports[i]);
	IMEMotorSetInit( self.flywheelMotors, ports, nMotors, gearbox );

	MovingAverageInit( self.maFlywheelSpeed, 6 );
	MovingAverageInit( self.maSpeedError, 4 );

	self.Kq = q;
	self.Ki = i;
	self.Kd = d;

	self.A = a;
	self.B = b;
}


float cruisingPower( FlywheelSpeedController& self ){
	if( self.targetSpeed < 1 ) return 0;
	return (self.A * exp( self.B * self.targetSpeed )) / MainBatteryVoltage();
	//getAverage( maBattery );
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



void update( FlywheelSpeedController& self ){
	//nextSample( maBattery, MainBatteryVoltage() );

	// Measure the instantaneous motor speed, and update the average speed
	measure( self.flywheelMotors );
	nextSample( self.maFlywheelSpeed, fabs(self.flywheelMotors.ime.velocity) );
	float measuredSpeed = getAverage( self.maFlywheelSpeed );

	float previousError = self.speedError;

	self.speedError = measuredSpeed - self.targetSpeed;
	float eSquared = self.speedError * self.speedError;
	float dt = self.flywheelMotors.ime.dt;
	if( dt == 0 ) dt = 1;

	float qTerm = sgn(self.speedError)*eSquared;
	float iTerm = self.maSpeedError.sum;
	float dTerm = (self.speedError - previousError) / dt;

	setPower( self, cruisingPower(self) - (self.Kq*qTerm + self.Kd*dTerm + self.Ki*iTerm) );
}

#endif
