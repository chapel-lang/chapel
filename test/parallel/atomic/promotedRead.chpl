// This is a unit test that ensures promoting a read on an array of atomics
// works.
var arrAtom: [0..3] atomic int;
arrAtom[1].add(3);
var promoRead = arrAtom.read();
writeln(promoRead);
