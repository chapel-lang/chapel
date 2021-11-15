use Set;

var sa = new set([0..2] int);

sa.add([1, 2, 3]);

writeln(sa);

// Would like to enable this as well, but we don't currently support
// hash functions on domains (#11340):
//
// var sd = new set(domain(1));
// sd.add({0..2});
// writeln(sd);
