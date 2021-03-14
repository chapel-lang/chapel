use BlockDist;
use LayoutCS;

config const N = 16;


config type sparseLayoutType = DefaultDist;

const FullDom = {0..#N, 0..#N} dmapped Block({0..#N, 0..#N},
    sparseLayoutType=sparseLayoutType);

var FullSparseDom: sparse subdomain(FullDom);
var FullSparseArr: [FullSparseDom] int;

//define a hardcoded DefaultSparse subdomain for second quadrant
const SecondQuadrant = {0..#N/2, 0..#N/2};
var SparseSQ: sparse subdomain(SecondQuadrant);

//create diagonal indices
var diagIndArr1 : [{0..#N}] 2*int;
for i in SecondQuadrant.dim(0) {
  diagIndArr1[i*2] = (i, i);
  diagIndArr1[i*2+1] = (i, N/2-1-i);
}

SparseSQ += diagIndArr1;

//define a hardcoded CSR subdomain for second quadrant
const FourthQuadrant = {N/2..N-1, N/2..N-1};
var SparseFQ: sparse subdomain(FourthQuadrant) dmapped CS();

//create diagonal indices
var diagIndArr2 : [{0..#N}] 2*int;
for i in FourthQuadrant.dim(0) {
  diagIndArr2[(i-N/2)*2] = (i, i);
  diagIndArr2[(i-N/2)*2+1] = (i, N-1-(i-N/2));
}

SparseFQ += diagIndArr2;

FullSparseDom += SparseSQ;
FullSparseArr=1;

for i in FullDom.dim(0) {
  for j in FullDom.dim(1) {
    write(FullSparseArr[i, j]," ");
  }
  writeln();
}

FullSparseDom += SparseFQ;
FullSparseArr=2;

for i in FullDom.dim(0) {
  for j in FullDom.dim(1) {
    write(FullSparseArr[i, j]," ");
  }
  writeln();
}

var containsAll = true;
var matchingLocalSubdomains = true;
var localBag : [Locales.domain] domain(FullSparseDom.rank*FullSparseDom.idxType);
var fullBag : domain(FullSparseDom.rank*FullSparseDom.idxType);
var unionBag : domain(FullSparseDom.rank*FullSparseDom.idxType);
var intersectionBag : domain(FullSparseDom.rank*FullSparseDom.idxType);

for i in FullSparseDom do {
  containsAll &= FullSparseDom.contains( i );
  fullBag += i;
}
writeln( containsAll );

for onLocale in Locales {
  on onLocale {
    for localIndex in FullSparseDom.dsiLocalSubdomain(here) {
      localBag[ onLocale.id ] += localIndex;
    }
    matchingLocalSubdomains &= ( FullSparseDom.dsiLocalSubdomain(here) == FullSparseArr.dsiLocalSubdomain(here) );
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
