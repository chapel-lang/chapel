use Random;

config const useNPB = true;
config const getNth = true;

var npbRand = makeRandomStream(real, 314159265, algorithm=RNG.NPB);
var pcgRand = makeRandomStream(real, 314159265, algorithm=RNG.PCG);

if useNPB {
  if getNth {
    npbRand.getNth(-1);
  } else {
    npbRand.skipToNth(-1);
  }
} else {
  if getNth {
    pcgRand.getNth(-1);
  } else {
    pcgRand.skipToNth(-1);
  }
}
