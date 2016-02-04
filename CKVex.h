#ifndef __CKVex__
#define __CKVex__

#pragma systemFile



// Use a floating point number in Volts by default
#define MainBatteryVoltage()	(nAvgBatteryLevel / 1000.0)


#define isPressed(port)		(!SensorValue[(port)])


#ifdef VEX2 // Vex (EDR) Cortex 2.0

	#define JoyChLX Ch4
	#define JoyChLY Ch3
	#define JoyChRX Ch1
	#define JoyChRY Ch2

	#define MotorPowerMax			127.0
	#define	ControllerJoyMax	127.0

	//TODO: will this ever apply to cortex?
	#define DrawSizeX		10
	#define DrawSizeY		10


	enum Motor393GearBox {
		M393Standard,
		M393HighSpeed,
		M393Turbo
	};


	#define TicksPerRev_393Standard			627.2
	#define TicksPerRev_393HighSpeed		392
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


	#define JoyChLX ChB
	#define JoyChLY ChA
	#define JoyChRX ChC
	#define JoyChRY ChD

	#define MotorPowerMax			100.0
	#define	ControllerJoyMax	100.0

	#define DrawSizeX		128
	#define DrawSizeY		 44


//=====================
// END Vex IQ
//=====================
#endif






#endif
