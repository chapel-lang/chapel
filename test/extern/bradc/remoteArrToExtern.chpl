use SysCTypes;

require "cprintarr.h";

var A: [1..10] real;

extern proc cprintarr(X: [] real, size: c_int);

on Locales[numLocales-1] do
  cprintarr(A, 10);
