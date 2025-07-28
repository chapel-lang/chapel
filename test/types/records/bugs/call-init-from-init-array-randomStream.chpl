use Random;
// make sure there is no infinite loop in the compiler when
// resolving randomStream's init function
var rng2 = new randomStream(int);
var rng: [0..4] randomStream(int);
// only print the eltType, since the seed is random and specifying would break
// the point of this test
writeln(rng2.eltType:string);
writeln(rng[2].eltType:string);
