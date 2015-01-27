extern type fftw_complex = 2*real(64);
extern proc test(ref x : fftw_complex);
type t2 = c_ptr(complex(128));
extern proc test2(x : t2);

var a : fftw_complex = (2,1);
test(a);
var b : complex = 1.0+2.0i;
writeln(b);
test2(c_ptrTo(b):t2);
writeln(b);

