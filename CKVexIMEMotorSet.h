#ifndef __CKVexIMEMotorSet__
#define __CKVexIMEMotorSet__

#pragma systemFile


#include <CKVexIME.h>
#include <CKVexMotorSet.h>




struct IMEMotorSet {
	MotorSet motors;
	IME ime; // assume that the 0th port in motors is the IME'd motor
};





void IMEMotorSetInit( IMEMotorSet& self, tMotor* ports, int nMotors, Motor393GearBox gearbox = M393Standard ){
	MotorSetInit( self.motors, ports, nMotors );
	IMEInit( self.ime, ports[0], gearbox );
}





void setPower( IMEMotorSet& self, float power ){
	setPower( self.motors, power );
}





void measure( IMEMotorSet& self ){
	measure( self.ime );
}





void waitUntilRest( IMEMotorSet& self ){
	waitUntilRest( self.ime );
}





#endif
