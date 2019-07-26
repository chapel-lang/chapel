var parentDom1D = {1..100};
var sparseDom1D: sparse subdomain(parentDom1D);
const indexRange = 10..90 by 20;

iter indexGen(d) {
  if d.rank == 1 {
    for i in indexRange do yield i;
  }
  else if d.rank == 2 {
    for i in {indexRange, indexRange} do yield i;
  }
  else {
    halt("Nope");
  }
}

{
  var spsIndexBuffer = sparseDom1D.makeIndexBuffer(size=10);
  for i in indexGen(sparseDom1D) do
    spsIndexBuffer.add(i);
}

writeln("Index range :", indexRange);
writeln("Sparse domain :", sparseDom1D);
