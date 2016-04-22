#ifndef __CKVexIME__
#define __CKVexIME__

#pragma systemFile


#include <CKVex.h>


const float TicksPerRev[] = {
	TicksPerRev_393Standard,
	TicksPerRev_393HighSpeed,
	TicksPerRev_393Turbo
};

const float RadiansPerTick[] = {
	2*PI / TicksPerRev_393Standard,
	2*PI / TicksPerRev_393HighSpeed,
	2*PI / TicksPerRev_393Turbo
};




struct IME {
	tMotor port; // port of the motor with the IME
	Motor393GearBox gearbox;

	float position; // radians
	float velocity; // radians per second
	float acceleration; // radians/second^2

	long time0; // Used for calculating velocity
	float dt; // time since last update (seconds)
};





void IMEInit( IME& self, tMotor port, Motor393GearBox gearbox = M393Standard ){
	self.port = port;
	self.gearbox = gearbox;

	self.position = 0;
	self.velocity = 0;
	self.acceleration = 0;

	self.time0 = nPgmTime;

	nMotorEncoder[ self.port ] = 0;
}




void setGearbox( IME& self, Motor393GearBox gearbox ){
	self.gearbox = gearbox;
}





void measure( IME& self ){

	// Take a timestamp ASAP before reading the encoder count,
	// so that they are as closely corresponding as possible.
	long now = nPgmTime;

	// Change in angular position since last time (radians).
	float dx = nMotorEncoder[ self.port ] * RadiansPerTick[ self.gearbox ];

	// Important to reset the encoder count ASAP after reading it.
	nMotorEncoder[ self.port ] = 0;

	// Keep track of the absolute position in float radians form.
	self.position += dx;

	// time period since last time (seconds).
	self.dt = (float)(now - self.time0) / 1000.0;

	// We'll be dividing by dt, so...
	if( self.dt == 0 ) self.dt = 1;

	// Save the previous velocity measurement before we overwrite it.
	float velocity0 = self.velocity;

	// Calculate the new velocity (radians per second).
	self.velocity = dx / self.dt;

	// Change in velocity so we can have an acceleration.
	float dv = self.velocity - velocity0;
	self.acceleration = dv / self.dt;

	// Save the timestamp of this measurement for next time.
	self.time0 = now;
}





void waitUntilRest( IME& self ){
	int restCount = 0;
	while( restCount < 5 ){
		measure( self );
		if( self.velocity < 0.001 )
			++restCount;
		else
			restCount = 0;
		delay(50);
	}
}





#endif
