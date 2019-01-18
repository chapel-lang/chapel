use BlockDist;

config const N=100;

const ParentDom = {0..#N} dmapped Block({0..#N});
var SparseDom: sparse subdomain(ParentDom);

//domain for multiples of 5
var FSparseDom: sparse subdomain(ParentDom);
for i in ParentDom by 5 {
  FSparseDom += i;
}

//domain for multiples of 7
var SSparseDom: sparse subdomain(ParentDom);
for i in ParentDom by 7 {
  SSparseDom += i;
}

SparseDom += FSparseDom;
SparseDom += SSparseDom;

for i in SparseDom do writeln(i);

var containsAll = true;
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
  }
}

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
