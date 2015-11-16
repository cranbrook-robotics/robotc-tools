
float maxOf( float* arr, const int N ) {
	float m = N > 0 ? arr[0] : 0;
	for( int i = 1; i < N; ++i ){
		if( arr[i] > m ) m = arr[i];
	}
	return m;
}
