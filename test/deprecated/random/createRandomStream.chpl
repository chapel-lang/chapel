use Random;

config param rtype = RNG.PCG;

var rnd  = createRandomStream(real, seed=314159265, algorithm=rtype);
