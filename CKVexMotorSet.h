#ifndef __CKVexMotorSet__
#define __CKVexMotorSet__

#pragma systemFile

#include <CKVex.h>



#ifndef CKMotorSetCapacity
#define CKMotorSetCapacity 12
#endif




struct MotorSet {
	tMotor ports[CKMotorSetCapacity];
	int NMotors;
	float power; // proportional; in [-1 to 1]
};





void updatePower( MotorSet& self );

void MotorSetInit( MotorSet& self, tMotor* ports, int nMotors ){
	ASSERT( nMotors <= CKMotorSetCapacity );

	self.NMotors = nMotors;
	for( int i = 0; i < self.NMotors; ++i )
		self.ports[i] = ports[i];
	updatePower( self );
}





void updatePower( MotorSet& self ){
	int power = motorPower( self.power );
	for( int i = 0; i < self.NMotors; ++i )
		motor[ self.ports[i] ] = power;
}





void setPower( MotorSet& self, float power ){
	self.power = bound( power, -1, 1 );
	updatePower( self );
}





void gradualStop( MotorSet& self ){
	float s = sgn( self.power );
	//for( float p = fabs(self.power); p > 0; p -= 0.01 ){
	//	setPower( self, s*p );
	//	delay(20);
	//}
	setPower( self, 0 );
}





#endif
