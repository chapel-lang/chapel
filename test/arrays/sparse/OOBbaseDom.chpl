use LayoutCS;

config const useCS = false;

const D = {1..10, 1..10};

var SDdefault: sparse subdomain(D);
var SDCS: sparse subdomain(D) dmapped CS();

if useCS then
  foo(SDCS);
else
  foo(SDdefault);

proc foo(ref SD) {
  SD += (1,1);
  SD += (5,3);

  var A: [SD] real;

  for ij in SD do
    writeln("A[",ij,"] = ", A[ij]);

  SD += (0,0);

  for ij in SD do
    writeln("A[",ij,"] = ", A[ij]);
}
