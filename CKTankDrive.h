#ifndef __CKTankDrive__
#define __CKTankDrive__

#pragma systemFile

#include <CKVexMotorSet.h>

struct TankDrive {
	MotorSet LEFTDRIVE, RIGHTDRIVE;
	tSensors LEFTENCODER, RIGHTENCODER;
	tSensors GYROSCOPE;
	int WHEELDIAMETER;
	float KPDRIVE;
}

void tankDriveInit (TankDrive& self, MotorSet lSide, MotorSet rSide, tSensors lEnc, tSensors rEnc, tSensors mainGyro, int wheelD, float newKP)
{
	self.LEFTDRIVE = lSide;
	self.RIGHTDRIVE = rSide;
	self.LEFTENCODER = lEnc;
	self.RIGHTENCODER = rEnc;
	self.GYROSCOPE = mainGyro;
	self.WHEELDIAMETER = wheelD;
	self.KPDRIVE = newKP;
}

void setDriveTank (TankDrive& self, word lStick, word rStick)
{
		setPower(self.LEFTDRIVE, vexRT[lStick]/127.);
		setPower(self.RIGHTDRIVE, vexRT[rStick]/127.);
}

void driveT (TankDrive& self, bool forward, int time)
{
	setPower(self.LEFTDRIVE, forward ? 1 : -1);
	setPower(self.RIGHTDRIVE, forward ? 1 : -1);
	delay(time);
	setPower(self.LEFTDRIVE, forward ? -0.15 : 0.15);
	setPower(self.RIGHTDRIVE, forward ? -0.15 : 0.15);
	delay(40);
	setPower(self.LEFTDRIVE, 0);
	setPower(self.RIGHTDRIVE, 0);
}

void driveForward (TankDrive& self, float inchesToDrive)
{

	float ticksToDrive = 360 * inchesToDrive / (2 * PI * self.WHEELDIAMETER);
	SensorValue(self.LEFTENCODER) = 0;
	SensorValue(self.RIGHTENCODER) = 0;
	int rightEncoderCount = 0, leftEncoderCount = 0;
	bool rightIsDone = false, leftIsDone = false;
	SensorValue(self.GYROSCOPE) = 0;
	int gyroReading = SensorValue[self.GYROSCOPE];
	while (!rightIsDone || !leftIsDone)
	{
		rightIsDone = rightEncoderCount>=ticksToDrive;
		leftIsDone = leftEncoderCount>=ticksToDrive;
		gyroReading = SensorValue[self.GYROSCOPE];
		rightEncoderCount += SensorValue[self.RIGHTENCODER];
		SensorValue(self.RIGHTENCODER) = 0;
		leftEncoderCount += SensorValue[self.LEFTENCODER];
		SensorValue(self.LEFTENCODER) = 0;
		float error = gyroReading;
		setPower(self.RIGHTDRIVE, rightIsDone ? 0 : bound(0.75 - self.KPDRIVE*error, 0, 1));
		setPower(self.LEFTDRIVE, leftIsDone ? 0 : bound(0.75 + self.KPDRIVE*error, 0, 1));
	}
	setPower(self.RIGHTDRIVE, -0.15);
	setPower(self.LEFTDRIVE, -0.15);
	delay(40);
	setPower(self.RIGHTDRIVE, 0);
	setPower(self.LEFTDRIVE, 0);
}

void driveBackward (TankDrive& self, float inchesToDrive)
{
	float ticksToDrive = -360 * inchesToDrive / (2 * PI * self.WHEELDIAMETER);
	SensorValue(self.LEFTENCODER) = 0;
	SensorValue(self.RIGHTENCODER) = 0;
	int rightEncoderCount = 0, leftEncoderCount = 0;
	bool rightIsDone = false, leftIsDone = false;
	SensorValue(self.GYROSCOPE) = 0;
	int gyroReading = SensorValue[self.GYROSCOPE];
	while (!rightIsDone || !leftIsDone)
	{
		rightIsDone = rightEncoderCount<=ticksToDrive;
		leftIsDone = leftEncoderCount<=ticksToDrive;
		gyroReading = SensorValue[self.GYROSCOPE];
		rightEncoderCount += SensorValue[self.RIGHTENCODER];
		SensorValue(self.RIGHTENCODER) = 0;
		leftEncoderCount += SensorValue[self.LEFTENCODER];
		SensorValue(self.LEFTENCODER) = 0;
		float error = gyroReading;
		setPower(self.RIGHTDRIVE, rightIsDone ? 0 : bound(-0.75 - self.KPDRIVE*error, -1, 0));
		setPower(self.LEFTDRIVE, leftIsDone ? 0 : bound(-0.75 + self.KPDRIVE*error, -1, 0));
	}
	setPower(self.RIGHTDRIVE, 0.15);
	setPower(self.LEFTDRIVE, 0.15);
	delay(40);
	setPower(self.RIGHTDRIVE, 0);
	setPower(self.LEFTDRIVE, 0);
}

void turnRight (TankDrive& self, float degreesToTurn)
{
	int deciDegreesToTurn = (degreesToTurn-6) * 10;
	SensorValue[self.GYROSCOPE] = 0;
	int gyroReading = -SensorValue[self.GYROSCOPE];
	while (gyroReading != deciDegreesToTurn)
	{
		gyroReading = -SensorValue[self.GYROSCOPE];
		if(gyroReading < deciDegreesToTurn){
			setPower(self.RIGHTDRIVE, -0.5);
			setPower(self.LEFTDRIVE, 0.5);
		}
		if(gyroReading > deciDegreesToTurn){
			setPower(self.RIGHTDRIVE, 0.5);
			setPower(self.LEFTDRIVE, -0.5);
		}
	}
	setPower(self.LEFTDRIVE, 0.05);
	setPower(self.RIGHTDRIVE, -0.05);
	delay(40);
	setPower(self.LEFTDRIVE, 0);
	setPower(self.RIGHTDRIVE, 0);
}

void turnLeft (TankDrive& self, float degreesToTurn)
{
	int deciDegreesToTurn = (degreesToTurn-6) * 10;
	SensorValue[self.GYROSCOPE] = 0;
	int gyroReading = SensorValue[self.GYROSCOPE];
	while (gyroReading != deciDegreesToTurn)
	{
		gyroReading = SensorValue[self.GYROSCOPE];
		if (gyroReading < deciDegreesToTurn)
		{
			setPower(self.RIGHTDRIVE, 0.5);
			setPower(self.LEFTDRIVE, -0.5);
		}
		if (gyroReading > deciDegreesToTurn)
		{
			setPower(self.RIGHTDRIVE, -0.5);
			setPower(self.LEFTDRIVE, 0.5);
		}
	}
	setPower(self.LEFTDRIVE, -0.05);
	setPower(self.RIGHTDRIVE, 0.05);
	delay(40);
	setPower(self.LEFTDRIVE, 0);
	setPower(self.RIGHTDRIVE, 0);
}

#endif
