use BlockDist;
use LayoutCS;
use List;

var parentDom1D = {1..100};
var parentDom2D = {1..100, 1..100};
var distParentDom1D = {1..100} dmapped Block({1..100});
var distParentDom2DCOO = {1..100, 1..100} dmapped Block({1..100, 1..100});
var distParentDom2DCSR = {1..100, 1..100} dmapped Block({1..100, 1..100},
    sparseLayoutType=CS(compressRows=true));
var distParentDom2DCSC = {1..100, 1..100} dmapped Block({1..100, 1..100},
    sparseLayoutType=CS(compressRows=false));

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

proc test(d) {
  var spsIndexBuffer = d.createIndexBuffer(size=4);
  for i in indexGen(d) do
    spsIndexBuffer.add(i);
  spsIndexBuffer.commit();
  printDomain(d);
}

proc printDomain(d) {
  var indexList: list(if d.rank==1 then d.idxType else d.rank*d.idxType);
  for i in d do
    indexList.pushBack(i);
  indexList.sort();
  writeln(indexList);
}

// test local 1D domains
var cooDom1D: sparse subdomain(parentDom1D);
test(cooDom1D);

// test local 2D domains
var cooDom2D: sparse subdomain(parentDom2D);
test(cooDom2D);
var csrDom: sparse subdomain(parentDom2D) dmapped CS(compressRows=true);
test(csrDom);
var cscDom: sparse subdomain(parentDom2D) dmapped CS(compressRows=false);
test(cscDom);

// test distributed 1D domains
var cooDom1DDist: sparse subdomain(distParentDom1D);
test(cooDom1DDist);

// test distributed 2D domains
var cooDom2DDist: sparse subdomain(distParentDom2DCOO);
test(cooDom2DDist);
var csrDom2DDist: sparse subdomain(distParentDom2DCSR);
test(csrDom2DDist);
var cscDom2DDist: sparse subdomain(distParentDom2DCSC);
test(cscDom2DDist);
