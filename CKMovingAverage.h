#ifndef __CKMovingAverage__
#define __CKMovingAverage__

#pragma systemFile



#ifndef CKMovingAverageSize
#define CKMovingAverageSize 5
#endif


#ifndef CKMovingAverageSampleType
#define CKMovingAverageSampleType float
#endif


struct MovingAverage {
	CKMovingAverageSampleType samples[CKMovingAverageSize];
	unsigned int iSample;
	CKMovingAverageSampleType sum;
};



void MovingAverageInit( MovingAverage& self ){
	memset( self.samples, 0, sizeof(CKMovingAverageSampleType)*CKMovingAverageSize );
	self.sum = 0;
	self.iSample = 0;
}


void nextSample( MovingAverage& self, CKMovingAverageSampleType sample ){
	self.sum += sample - self.samples[self.iSample];
	self.samples[self.iSample] = sample;
	self.iSample = (self.iSample + 1) % CKMovingAverageSize;
}


CKMovingAverageSampleType getAverage( MovingAverage& self ){
	return self.sum / CKMovingAverageSize;
}



#endif
