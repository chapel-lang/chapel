use IO;
use Set;

// constructors work
const empty = ioHintSet.empty;
const seq = ioHintSet.sequential;
const rand = ioHintSet.random;
const pref = ioHintSet.prefetch;
const mmap = ioHintSet.mmap;

// set operations work
writeln(empty == seq & rand & pref & mmap);
writeln(seq | rand != pref | mmap);
writeln(seq != seq | rand);
writeln(pref & pref | rand == pref | rand);

// internal bits are unique
var s = new set(int(32));
s.add(empty._internal);
s.add(seq._internal);
s.add(rand._internal);
s.add(pref._internal);
s.add(mmap._internal);
writeln(s.size);
