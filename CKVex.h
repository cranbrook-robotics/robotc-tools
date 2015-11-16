#ifndef __CKVex__
#define __CKVex__


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


#endif
