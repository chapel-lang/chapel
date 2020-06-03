use BlockDist;
use LayoutCS;

config const N = 8;

config type sparseLayoutType = DefaultDist;
const ParentDom = {0..#N, 0..#N} dmapped Block({0..#N, 0..#N},
    sparseLayoutType=sparseLayoutType);

var SparseDom: sparse subdomain(ParentDom);

var SparseMat: [SparseDom] int;

//create a small dense chunk somewhere in vector
const denseChunk = {1..3, 5..7};

SparseDom += denseChunk; //not sure if this would work

for i in denseChunk do SparseMat[i]=i[0]+i[1];

writeln("Dense chunk:");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseMat[i,j], " ");
  }
  writeln();
}

//create a strided chunk
var stridedChunk = {1..6 by 2, 1..3};

SparseDom += stridedChunk;

for i in stridedChunk do SparseMat[i]=i[0]+i[1];

writeln("Dense + strided chunk:");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseMat[i,j], " ");
  }
  writeln();
}

//create diagonal indices
var diagIndArr : [{0..#2*N}] 2*int;
for i in ParentDom.dim(0) {
  diagIndArr[i*2] = (i, i);
  diagIndArr[i*2+1] = (i, N-1-i);
}

SparseDom += diagIndArr;

for i in diagIndArr do SparseMat[i]=i[0]+i[1];

writeln("Chunks + Diagonals:");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseMat[i,j], " ");
  }
  writeln();
}

var containsAll = true;
var matchingLocalSubdomains = true;
var localBag : [Locales.domain] domain(SparseDom.rank*SparseDom.idxType);
var fullBag : domain(SparseDom.rank*SparseDom.idxType);
var unionBag : domain(SparseDom.rank*SparseDom.idxType);
var intersectionBag : domain(SparseDom.rank*SparseDom.idxType);

for i in SparseDom do {
  containsAll &= SparseDom.contains( i );
  fullBag += i;
}
writeln( containsAll );

for onLocale in Locales {
  on onLocale {
    for localIndex in SparseDom.dsiLocalSubdomain(here) {
      localBag[ onLocale.id ] += localIndex;
    }
    matchingLocalSubdomains &= ( SparseDom.dsiLocalSubdomain(here) == SparseMat.dsiLocalSubdomain(here) );
  }
}

writeln( matchingLocalSubdomains );

intersectionBag = fullBag;

for onLocale in Locales {
  for i in localBag[ onLocale.id ] {
    unionBag |= localBag[ onLocale.id ];
    intersectionBag &= localBag[ onLocale.id ];
  }
}
// union of disparate sets is the full set
writeln( unionBag == fullBag );
// intersection of disparate sets is the empty set
// ... but only if there is more than one locale
if Locales.size > 1
  then writeln( intersectionBag.size == 0 );
  else writeln( true );
