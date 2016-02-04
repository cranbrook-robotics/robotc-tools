#ifndef __CKVex__
#define __CKVex__

#pragma systemFile


#include <CKGeneral.h>



// Use a floating point number in Volts by default
#define MainBatteryVoltage()	(nAvgBatteryLevel / 1000.0)


#define isPressed(port)		(!SensorValue[(port)])


#ifdef VEX2 // Vex (EDR) Cortex 2.0

	#define ChJoyLX Ch4
	#define ChJoyLY Ch3
	#define ChJoyRX Ch1
	#define ChJoyRY Ch2

	#define FullMotorPower	127.0
	#define	FullJoystick		127.0

	//TODO: will this ever apply to cortex?
	#define DrawSizeX		10
	#define DrawSizeY		10


	enum Motor393GearBox {
		M393Standard,
		M393HighSpeed,
		M393Turbo
	};


	#define TicksPerRev_393Standard			627.2
	#define TicksPerRev_393HighSpeed		392.0
	#define TicksPerRev_393Turbo				261.333

	#define Standard393RPM		100
	#define HighSpeed393RPM		160
	#define Turbo393RPM				240


	#define SingleWireEncoderTicksPerRev	180


	void uartPrintln(TUARTs uartPort, const char* str){
		const int N = strlen(str);
		for( int i = 0; i < N; ++i ){
			sendChar(uartPort, str[i]);
		}
		sendChar(uartPort, '\n');
		sendChar(uartPort, '\r');
	}



	float potentiometer( tSensors port ){
		return SensorValue[port] / 4095.0;
	}

//=====================
// END Vex Cortex 2.0
//=====================

#else if defined(VexIQ)


	#define ChJoyLX ChB
	#define ChJoyLY ChA
	#define ChJoyRX ChC
	#define ChJoyRY ChD

	#define FullMotorPower	100.0
	#define	FullJoystick		100.0

	#define DrawSizeX		128
	#define DrawSizeY		 44


//=====================
// END Vex IQ
//=====================
#endif




int motorPower( float proportionalPower ){
	return (int)round( bound(proportionalPower, -1, 1) * FullMotorPower );
}



void setMotorPower( tMotor m, float proportionalPower ){
	motor[m] = motorPower( proportionalPower );
}




float joystick( TVexJoysticks joyChannel ){
	return (float)getJoystickValue(joyChannel) / FullJoystick;
}



#endif
