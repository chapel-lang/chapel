iter blockByCol(m,n) {
  for i in 1..n by 2 do yield {1..n, i..i+1};
}  

const n = 4;
const D = {1..n,1..n};
var A: [D] int;

[ij in D] A(ij) = ij(1);

for block in blockByCol(2,4) {
  writeln("This block:");
  writeln(A(block));
}
