use LayoutCS;

config const useCS = false;

const D = {1..10, 1..10};
const D2 = D by 2;
var SDdefault: sparse subdomain(D2);
var SDCS: sparse subdomain(D2) dmapped CS();

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

  SD += (2,4);

  for ij in SD do
    writeln("A[",ij,"] = ", A[ij]);
}
