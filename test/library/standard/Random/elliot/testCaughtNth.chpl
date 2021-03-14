use Random;

config const useNPB = true;
config const getNth = true;

var npbRand = createRandomStream(real, 314159265, algorithm=RNG.NPB);
var pcgRand = createRandomStream(real, 314159265, algorithm=RNG.PCG);

try! {
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
} catch (e: IllegalArgumentError) {
  writeln("Successfully caught IllegalArgumentError");
}
