#ifndef __CKGeneral__
#define __CKGeneral__

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

#define bound(x, a, b)	(min( max(x, a), b ))

float maxOf( float* arr, const int N );


//=============================================================
// This is bad practice for professional software development,
// but it will have to do for now.

#include "CKGeneral.c"


#endif
