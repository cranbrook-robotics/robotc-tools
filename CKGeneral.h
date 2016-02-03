#ifndef __CKGeneral__
#define __CKGeneral__

#pragma systemFile


#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

#define bound(x, a, b)	(min( max(x, a), b ))



float maxOf( float* arr, const int N ) {
	float m = N > 0 ? arr[0] : 0;
	for( int i = 1; i < N; ++i ){
		if( arr[i] > m ) m = arr[i];
	}
	return m;
}



#endif
