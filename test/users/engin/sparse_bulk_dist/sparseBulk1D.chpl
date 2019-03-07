use BlockDist;
config const N = 32;

const ParentDom = {0..#N} dmapped Block({0..#N});

var SparseDom: sparse subdomain(ParentDom);

var SparseVec: [SparseDom] int;

//create a small dense chunk somewhere in vector
var denseChunk = {7..14};

SparseDom += denseChunk; //not sure if this would work

//create a strided chunk
var stridedChunk = {18..28 by 2};

SparseDom += stridedChunk;

for (d,a) in zip(SparseDom, SparseVec) do a=d;

for i in ParentDom do writeln(SparseVec[i]);

//create some more interesting indices
var cnt = N/3;
var inds: [{0..#cnt}] int;
for (n,ind) in zip(0..#cnt,inds) do ind=n*3;
SparseDom += inds;

for (d,a) in zip(SparseDom, SparseVec) do a=d;

for i in ParentDom do writeln(SparseVec[i]);

//create even indices
var cnt2 = N/2-1;
var inds2: [{0..#cnt2}] int;
for (n, ind) in zip(0..#cnt2, inds2) do ind=n*2;
SparseDom += inds2;

for (d,a) in zip(SparseDom, SparseVec) do a=d;

for i in ParentDom do writeln(SparseVec[i]);

var containsAll = true;
var matchingLocalSubdomains = true;
var localBag : [Locales.domain] domain(SparseDom.idxType);
var fullBag : domain(SparseDom.idxType);
var unionBag : domain(SparseDom.idxType);
var intersectionBag : domain(SparseDom.idxType);

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
    matchingLocalSubdomains &= ( SparseDom.dsiLocalSubdomain(here) == SparseVec.dsiLocalSubdomain(here) );
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
