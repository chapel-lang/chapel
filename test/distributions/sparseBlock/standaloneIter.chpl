use BlockDist;

var A = newBlockArr({1..10 by 2, 2..10 by 2}, real);
var DS: sparse subdomain(A.domain);
var AS: [DS] real;

DS += (1,2);
DS += (1,4);
DS += (9,10);

forall a in AS do
  writeln(a);

