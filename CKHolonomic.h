#ifndef __CKVexMotorSet__
#define __CKVexMotorSet__

#pragma systemFile

#include <CKGeneral.h>
#include <CKVex.h>


const int MaxNWheels = 4;


struct HolonomicBase {
	int NWheels;
	tMotor MotorPorts[MaxNWheels];
	float WheelHeadings[MaxNWheels];

	float wheelPowers[MaxNWheels];

}



// TVexJoysticks JoyChDriveX = ChJoyLX;
// TVexJoysticks JoyChDriveY = ChJoyLY;
// TVexJoysticks JoyChDriveR = ChJoyRX;




void InitHolonomicBase(HolonomicBase& self, tMotor* ports, int NWheels = 4) {

	// Zero them all in case we don't use all slots.
	memset( self.MotorPorts, 0, sizeof(tMotor) * MaxNWheels );
	memset( self.WheelHeadings, 0, sizeof(float) * MaxNWheels );

	self.NWheels = NWheels;
	for( int i = 0; i < self.NWheels; ++i ){
		self.MotorPorts[i] = ports[i];
	}

	// Compute the orientation of each wheel based on the assumption that
	// they are arranged in a regular polygonal form.
	float InitialWheelHeading = degreesToRadians( 360. / (self.NWheels * 2) );
	float headingIncrement = 2*PI / self.NWheels;
	for( int i = 0; i < self.NWheels; ++i ){
		self.WheelHeadings[i] = i * headingIncrement + InitialWheelHeading;
	}

}





void setDriveHMR(HolonomicBase& self, float driveHeading, float driveMagnitude, float driveRotation) {

	float maxPower = 0; // Keep track of the max power magnitude for later

	for( int i = 0; i < self.NWheels; ++i ){
		// Angle of this particular wheel with the target drive heading.
		// For example, 0 or 180 would mean that this wheel is perfectly aligned with
		// the translational drive direction, and 90 or 270 would mean that it is
		// perfectly perpendicular.
		float angleWithTarget = driveHeading - self.WheelHeadings[i];

		// Incorporate both the translational vector (relative to this wheel), as
		// well as the rotation.  This addition of the rotation is what causes
		// the motor powers to sometimes go over the full power amount.
		float thisPower = driveMagnitude * cos(angleWithTarget) + driveRotation;

		self.wheelPowers[i] = thisPower;

		// Check for a new maximum motor power magnitude.
		if( fabs(thisPower) > maxPower ){
			maxPower = fabs(thisPower);
		}
	}

	// If any of the calculated motor powers are greater than full power,
	// we need to reduce all motor powers by the same factor such that
	// the motor with the greatest power is given full power.
	float powerFactor = maxPower > 1.0 ? (1.0/maxPower) : 1.0;
	for( int i = 0; i < self.NWheels; ++i ){
		setMotorPower( self.MotorPorts[i], powerFactor * self.wheelPowers[i] );
	}
}




void setDriveXYR(HolonomicBase& self, float driveX, float driveY, float driveRotation) {

	// Transform the translational drive control vector from Cartesian
	// form into angle & magnitude form.
	float driveHeading = atan2(driveY, driveX);
	float driveMagnitude = sqrt( driveX*driveX + driveY*driveY );

	setDriveHMR(self, driveHeading, driveMagnitude, driveRotation);
}


// while(true){
// 	float robotHeading = getGyroHeading(pGyro);

// 	setMotorTarget(mpHeading, (int)robotHeading, 100);
// 	displayTextLine(1, "%d", (int)robotHeading);

// 	robotHeading = degreesToRadians(robotHeading);

// 	// First obtain the joystick components: X, Y, and R
// 	float driveY = joystick(JoyChDriveY);
// 	float driveX = joystick(JoyChDriveX);
// 	float driveRotation = -joystick(JoyChDriveR);
// 	// Negative of the the rotational joystick because leftward rotation
// 	// is actually positive rotation about the vertical axis.

// 	// Transform the translational drive control vector from Cartesian
// 	// form into angle & magnitude form.
// 	float driveHeading = atan2(driveY, driveX) + robotHeading;
// 	float driveMagnitude = sqrt( driveX*driveX + driveY*driveY );

// }



#endif
