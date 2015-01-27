#include "dummy.h"

void test(fftw_complex* in) {
	(*in)[0]=10;
    (*in)[1]=20;	
};


void test2(double _Complex* in) {
	*in = 20.0+30.0*I;
}
