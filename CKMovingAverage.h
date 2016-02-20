#ifndef __CKMovingAverage__
#define __CKMovingAverage__

#pragma systemFile



#ifndef CKMovingAverageCapacity
#define CKMovingAverageCapacity 8
#endif


#ifndef CKMovingAverageSampleType
#define CKMovingAverageSampleType float
#endif


struct MovingAverage {
	CKMovingAverageSampleType samples[CKMovingAverageCapacity];
	int NSamples;
	int iSample;
	CKMovingAverageSampleType sum;
};



void MovingAverageInit( MovingAverage& self, int nSamples ){
	ASSERT( nSamples <= CKMovingAverageCapacity );

	memset( self.samples, 0, sizeof(CKMovingAverageSampleType)*CKMovingAverageCapacity );

	self.NSamples = nSamples;
	self.sum = 0;
	self.iSample = 0;
}


void nextSample( MovingAverage& self, CKMovingAverageSampleType sample ){
	self.sum += sample - self.samples[self.iSample];
	self.samples[self.iSample] = sample;
	self.iSample = (self.iSample + 1) % self.NSamples;
}


CKMovingAverageSampleType getAverage( MovingAverage& self ){
	return self.sum / self.NSamples;
}



#endif
