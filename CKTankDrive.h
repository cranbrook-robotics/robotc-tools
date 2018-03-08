/*
//	Program (c) 2018 Justin Applefield
//	Software design by Justin Applefield, Cranbrook Kingswood '18
//	Orginially written for VRC: In The Zone
//	Originally used by Team 39Z: The Zip Ties
*/

#ifndef __CKTankDrive__
#define __CKTankDrive__

#pragma systemFile

#include <CKVexMotorSet.h>

struct TankDrive {
	MotorSet LEFTDRIVE;
	MotorSet RIGHTDRIVE;
	tSensors LEFTENCODER;
	tSensors RIGHTENCODER;
	tSensors GYROSCOPE;
	int WHEELDIAMETER;
	float KPDRIVE;
	int delayMS;
	int turnThreshold;
};


//Parameters:
//TankDrive& self - TankDrive variable to be initialized
//tMotor* lSide - array of motors for left side of drive train
//tMotor* rSide - array of motors for right side of drive train
//int motorsPerSide - number of motors on each side of drive train
//tSensors lEnc - Shaft Encoder for Left Side
//tSensors rEnc - Shaft Encoder for Right Side
//tSensors mainGyro - Gyroscope
//int wheelD - diameter of wheel (in inches)
//float newKP - coefficient for the proportional loop executed in driveForward and driveBackward functions

void TankDriveInit (TankDrive& self, tMotor* lSide, tMotor* rSide, int motorsPerSide, tSensors lEnc, tSensors rEnc, tSensors mainGyro, int wheelD, float newKP, int newdelayMS, int newTurnThreshold)
{
	MotorSetInit(self.LEFTDRIVE, lSide, motorsPerSide);
	MotorSetInit(self.RIGHTDRIVE, rSide, motorsPerSide);
	self.LEFTENCODER = lEnc;
	self.RIGHTENCODER = rEnc;
	self.GYROSCOPE = mainGyro;
	self.WHEELDIAMETER = wheelD;
	self.KPDRIVE = newKP;
	self.delayMS = newdelayMS;
	self.turnThreshold = newTurnThreshold;
}

//Parameters:
//threshold - amount of joystick movement before drive responds, in [0,127]
void setDriveTank (TankDrive& self, float threshold)
{
		setPower(self.LEFTDRIVE, (abs(vexRT[Ch3]) < threshold) ? 0 : vexRT[Ch3]/127.);
		setPower(self.RIGHTDRIVE, (abs(vexRT[Ch2]) < threshold) ? 0 : vexRT[Ch2]/127.);
}

//Parameters:
//bool forward - direction of drive; true = forward; false = backwards
//float powerLevel - amount of power input, in [0,1]
void driveT (TankDrive& self, bool forward, int time, float powerLevel)
{
	setPower(self.LEFTDRIVE, forward ? powerLevel : -1*bound(powerLevel, 0, 1));
	setPower(self.RIGHTDRIVE, forward ? powerLevel : -1*bound(powerLevel, 0, 1));
	delay(time);
	setPower(self.LEFTDRIVE, forward ? -0.15 : 0.15);
	setPower(self.RIGHTDRIVE, forward ? -0.15 : 0.15);
	delay(40);
	setPower(self.LEFTDRIVE, 0);
	setPower(self.RIGHTDRIVE, 0);
}

//Parameters:
//float inchesToDrive - number of inches to drive
//float basePower - starting power level to each side of drive, in [0,1]
void driveForward (TankDrive& self, float inchesToDrive, float basePower)
{

	float ticksToDrive = 360 * inchesToDrive / (2 * PI * self.WHEELDIAMETER);
	basePower = bound(basePower, 0, 0.9);
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
		setPower(self.RIGHTDRIVE, rightIsDone ? 0 : bound(basePower - self.KPDRIVE*error, 0, 1));
		setPower(self.LEFTDRIVE, leftIsDone ? 0 : bound(basePower + self.KPDRIVE*error, 0, 1));
		delay(self.delayMS);
	}
	setPower(self.RIGHTDRIVE, -0.15);
	setPower(self.LEFTDRIVE, -0.15);
	delay(40);
	setPower(self.RIGHTDRIVE, 0);
	setPower(self.LEFTDRIVE, 0);
}

void driveBackward (TankDrive& self, float inchesToDrive, float basePower)
{
	float ticksToDrive = -360 * inchesToDrive / (2 * PI * self.WHEELDIAMETER);
	basePower = bound(basePower, 0, 0.9);
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
		setPower(self.RIGHTDRIVE, rightIsDone ? 0 : bound(-1*basePower - self.KPDRIVE*error, -1, 0));
		setPower(self.LEFTDRIVE, leftIsDone ? 0 : bound(-1*basePower + self.KPDRIVE*error, -1, 0));
		delay(self.delayMS);
	}
	setPower(self.RIGHTDRIVE, 0.15);
	setPower(self.LEFTDRIVE, 0.15);
	delay(40);
	setPower(self.RIGHTDRIVE, 0);
	setPower(self.LEFTDRIVE, 0);
}

void turnRight (TankDrive& self, float degreesToTurn, float basePower)
{
	int deciDegreesToTurn = degreesToTurn * 10;
	basePower = bound(basePower, 0, 1);
	SensorValue[self.GYROSCOPE] = 0;
	int gyroReading = -SensorValue[self.GYROSCOPE];
	while (abs(gyroReading - deciDegreesToTurn) > self.turnThreshold)
	{
		gyroReading = -SensorValue[self.GYROSCOPE];
		if(gyroReading < deciDegreesToTurn){
			setPower(self.RIGHTDRIVE, -1*basePower);
			setPower(self.LEFTDRIVE, basePower);
		}
		if(gyroReading > deciDegreesToTurn){
			setPower(self.RIGHTDRIVE, basePower);
			setPower(self.LEFTDRIVE, -1*basePower);
		}
		delay(self.delayMS);
	}
	setPower(self.LEFTDRIVE, 0.05);
	setPower(self.RIGHTDRIVE, -0.05);
	delay(40);
	setPower(self.LEFTDRIVE, 0);
	setPower(self.RIGHTDRIVE, 0);
}

void turnLeft (TankDrive& self, float degreesToTurn, float basePower)
{
	int deciDegreesToTurn = degreesToTurn * 10;
	basePower = bound(basePower, 0, 1);
	SensorValue[self.GYROSCOPE] = 0;
	int gyroReading = SensorValue[self.GYROSCOPE];
	while (abs(gyroReading - deciDegreesToTurn) > self.turnThreshold)
	{
		gyroReading = SensorValue[self.GYROSCOPE];
		if (gyroReading < deciDegreesToTurn)
		{
			setPower(self.RIGHTDRIVE, basePower);
			setPower(self.LEFTDRIVE, -1*basePower);
		}
		if (gyroReading > deciDegreesToTurn)
		{
			setPower(self.RIGHTDRIVE, -1*basePower);
			setPower(self.LEFTDRIVE, basePower);
		}
		delay(self.delayMS);
	}
	setPower(self.LEFTDRIVE, -0.05);
	setPower(self.RIGHTDRIVE, 0.05);
	delay(40);
	setPower(self.LEFTDRIVE, 0);
	setPower(self.RIGHTDRIVE, 0);
}

#endif
