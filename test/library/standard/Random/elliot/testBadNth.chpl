use Random;

config const useNPB = true;
config const getNth = true;

var npbRand = NPBRandomStream.create(real, 314159265);
var pcgRand = PCGRandomStream.create(real, 314159265);

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
