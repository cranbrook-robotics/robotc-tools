#ifndef __CKGeneral__
#define __CKGeneral__

#pragma systemFile


#define minimum(a,b) (((a)<(b))?(a):(b))
#define maximum(a,b) (((a)>(b))?(a):(b))

#define bound(x, a, b)	(minimum( maximum(x, a), b ))



float maxOf( float* arr, const int N ) {
	float m = N > 0 ? arr[0] : 0;
	for( int i = 1; i < N; ++i ){
		if( arr[i] > m ) m = arr[i];
	}
	return m;
}



#endif
