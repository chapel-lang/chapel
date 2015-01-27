use SysCTypes;

extern type fftw_complex = 2*real(64);
type t = c_ptr(2*c_double);
extern proc test(x : t);

var a : fftw_complex = (2,1);
writeln(a);
test(c_ptrTo(a) : t);
writeln(a);
