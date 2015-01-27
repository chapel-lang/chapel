extern type fftw_complex = 2*real(64);
extern proc test(ref x : fftw_complex);

var a : fftw_complex = (2,1);
test(a);
