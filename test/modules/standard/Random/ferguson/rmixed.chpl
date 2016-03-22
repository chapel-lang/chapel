use Random;

var rng = makeRandomStream(seed=17, parSafe=false,
                           eltType=real(64), algorithm=RNG.PCG);

var v1 = rng.getNext();
writeln(v1.type:string);
var v2 = rng.getNext(uint(8));
writeln(v2.type:string);
var v3 = rng.getNext(uint(64));
writeln(v3.type:string);

