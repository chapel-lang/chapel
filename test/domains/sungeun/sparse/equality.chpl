config const debug = false;
config param rank = 4;
config const n = 17;

proc checkit(d1, d2) {
  if debug {
    writeln(d1);
    writeln(d2);
  }
  writeln(d1==d2);
  writeln(d1!=d2);
}

var init_range: rank*range;
var myIdx: [1..rank] rank*int;
for param i in 1..rank {
  init_range(i) = 1..n*i;
  for j in 1..rank do
    myIdx[j](i) = if i==j then n else n*i-1;
}

const DD1: domain(rank) = init_range;
const DD2: domain(rank) = init_range;
var D1: sparse subdomain(DD1);
var D2: sparse subdomain(DD2);

checkit(D1, D2);

for i in 1..rank {
  if rank > 1 then // this bites
    D1 += myIdx[i];
  else
    D1 += (...myIdx[i]);
}
checkit(D1, D2);

for i in 1..rank by -1 {
  if rank > 1 then // this bites
    D2 += myIdx[i];
  else
    D2 += (...myIdx[i]);
}
checkit(D1, D2);

D2.clear();
checkit(D1, D2);

D1.clear();
checkit(D1, D2);

