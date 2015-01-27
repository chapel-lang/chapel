use SysCTypes;

extern type fftw_complex = 2*real(64);
type t = c_ptr(2*c_double);
type t2 = c_ptr(complex(128));
extern proc test(x : t);
extern proc test2(x : t2);

var a : fftw_complex = (2,1);
writeln(a);
test(c_ptrTo(a) : t);
writeln(a);
var b : complex = 1.0+2.0i;
writeln(b);
test2(c_ptrTo(b):t2);
writeln(b);

