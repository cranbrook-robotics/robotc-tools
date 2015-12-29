#ifndef __CKAverager__
#define __CKAverager__


#ifndef CKAveragerSampleSize
#define CKAveragerSampleSize 4
#endif


#ifndef CKAveragerSampleType
#define CKAveragerSampleType float
#endif


struct Averager {
	CKAveragerSampleType samples[CKAveragerSampleSize];
	unsigned int iSample;
	CKAveragerSampleType sum;
};



void AveragerInit( Averager& self ){
	memset( self.samples, 0, sizeof(CKAveragerSampleType)*CKAveragerSampleSize );
	self.sum = 0;
	self.iSample = 0;
}


void addSample( Averager& self, CKAveragerSampleType sample ){
	self.sum += sample - self.samples[self.iSample];
	self.samples[self.iSample] = sample;
	self.iSample = (self.iSample + 1) % CKAveragerSampleSize;
}


CKAveragerSampleType average( Averager& self ){
	return self.sum / CKAveragerSampleSize;
}



#endif
