#ifndef __CKVex__
#define __CKVex__



// Use a floating point number in Volts by default
#define MainBatteryVoltage()	(nAvgBatteryLevel / 1000.0)


#define isPressed(port)		(!SensorValue[(port)])


#ifdef VEX2
// VEX EDR Cortex 2.0

	#define JoyChLX Ch4
	#define JoyChLY Ch3
	#define JoyChRX Ch1
	#define JoyChRY Ch2

	//TODO: 100 or 127?
	#define MotorSpeedMax			127.0
	#define	ControllerJoyMax	127.0

	//TODO: will this ever apply to cortex?
	#define DrawSizeX		10
	#define DrawSizeY		10

#else//==================
// VEX IQ

	#define JoyChLX ChB
	#define JoyChLY ChA
	#define JoyChRX ChC
	#define JoyChRY ChD

	#define MotorSpeedMax			100.0
	#define	ControllerJoyMax	100.0

	#define DrawSizeX		128
	#define DrawSizeY		 44

#endif




#define TicksPerRev_393Standard			627.2
#define TicksPerRev_393HighSpeed		392
#define TicksPerRev_393Turbo				261.333
#define Standard393TicksPerRev		627.2
#define HighSpeed393TicksPerRev		392
#define Turbo393TicksPerRev				261.333

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



#endif
