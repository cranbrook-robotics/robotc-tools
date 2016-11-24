#ifndef __CKHolonomic__
#define __CKHolonomic__

#pragma systemFile

#include <CKGeneral.h>
#include <CKVex.h>


const int MaxNWheels = 4;


struct HolonomicBase {
	int NWheels;
	tMotor MotorPorts[MaxNWheels];
	float WheelHeadings[MaxNWheels];

	float wheelPowers[MaxNWheels];

};



// TVexJoysticks JoyChDriveX = ChJoyLX;
// TVexJoysticks JoyChDriveY = ChJoyLY;
// TVexJoysticks JoyChDriveR = ChJoyRX;


/**************************************************************

  Example motor port setup for an X-Drive base
  (four omni-wheels at the corners of a rectangular chasis)

      ________
   3 /        \ 2
    /          \
    |          |        + x-axis
    |          |       ---------->
    \          /
   4 \________/ 1

  The array of motor ports passed into the Init method
  should be in the order illustrated above.
  The first motor port in the array is the rear right
  in the above example because it has a 45 degree angle
  heading with respect to the + x-axis, which is the
  lowest positive angle heading of all the wheels.
  The general rule is to arrange the array of motor ports
  in ascending order of their respective wheel headings.


  Thus, here is an example with a triangular base:

         2
        ___
       /   \
      /     \
     /       \
    /         \
  3 \_________/ 1


**************************************************************/





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
		float thisPower = driveMagnitude * cos(angleWithTarget) - driveRotation;

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



void setDriveHMRFieldBased(HolonomicBase& self, float driveHeading, float driveMagnitude, float driveRotation, float robotHeading) {
	setDriveHMR(self, driveHeading - robotHeading, driveMagnitude, driveRotation);
}




void setDriveXYRFieldBased(HolonomicBase& self, float driveX, float driveY, float driveRotation, float robotHeading) {

	// Transform the translational drive control vector from Cartesian
	// form into angle & magnitude form.
	float driveHeading = atan2(driveY, driveX);
	float driveMagnitude = sqrt( driveX*driveX + driveY*driveY );

	setDriveHMRFieldBased(self, driveHeading, driveMagnitude, driveRotation, robotHeading);
}




void setDriveXYR(HolonomicBase& self, float driveX, float driveY, float driveRotation) {
	setDriveXYRFieldBased(self, driveX, driveY, driveRotation, 0);
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
