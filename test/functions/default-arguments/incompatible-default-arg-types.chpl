proc canSupportReal(type x) param do return x == real(32) || x == real(64);
config type fooArgType = integral;
proc foo(param pi: fooArgType = 3.14) where canSupportReal(fooArgType) { writeln("pi = ", pi); }
proc foo(param pi: fooArgType = 3) { writeln("pi = ", pi); }
foo();
