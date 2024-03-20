use Random, Random.NPBRandom, Random.PCGRandom;

var npb = new owned NPBRandomStream(real, 123),
    pcg = new owned PCGRandomStream(real, 123);
