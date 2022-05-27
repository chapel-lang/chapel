use BlockDist;

var A = newBlockArr({1..10 by 2, 1..10 by 2}, real);
var DS: sparse subdomain(A.domain);
var AS: [DS] real;

DS += (1,1);
DS += (1,3);
DS += (9,9);

forall a in AS do
  writeln(a);

