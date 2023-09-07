// This test illustrates the locality of array and domain slicing
// and the impact of the order of operands in domain slicing.

use BlockDist;

const D = {0..3};

const tlA = [Locales[0], Locales[1], Locales[2], Locales[3]];
const tlB = [Locales[2], Locales[3], Locales[0], Locales[1]];

const DA = Block.createDomain(D, tlA);
const DB = Block.createDomain(D, tlB);

var A: [DA] int = 0..3;
writeln("A = ", A); // 0 1 2 3

forall a in A {
  writeln("A:     ", a, " ", a.locale, " here: ", here);
}

forall a in A[DB] {
  writeln("A[DB]: ", a, " ", a.locale, " here: ", here);
}

forall idx in DA[DB] {
  writeln("DA[DB]: ", idx, " here: ", here);
}

forall idx in DB[DA] {
  writeln("DB[DA]: ", idx, " here: ", here);
}
