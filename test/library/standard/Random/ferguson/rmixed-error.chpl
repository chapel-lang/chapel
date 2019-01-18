use Random;

var rng = makeRandomStream(seed=17, parSafe=false,
                           eltType=real(64), algorithm=RNG.PCG);

var v1 = rng.getNext();
writeln(v1.type:string);
var v2 = rng.getNext(complex);
writeln(v2.type:string);
