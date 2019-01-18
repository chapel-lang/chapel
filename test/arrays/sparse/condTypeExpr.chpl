use LayoutCS;

config param useCS = true;

const D = {1..10, 1..10};
const D2 = D by 2;
var SD: if useCS then sparse subdomain(D2) dmapped CS()
                 else sparse subdomain(D2);

SD += (1,1);
SD += (5,3);

var A: [SD] real;

for ij in SD do
  writeln("A[",ij,"] = ", A[ij]);
