use Random;

config const n = 10000;
config const seed = 1;

proc createRandomArraySerial(minimum:int, maximum:int) {
  var A:[0..#n] int;
  var rng = createRandomStream(eltType=int, seed=seed);
  for i in 0..#n {
    A[i] = rng.getNext(minimum, maximum);
  }
  return A;
}

proc createRandomArrayParallel(nTasks:int, minimum:int, maximum:int) {
  var nPerTask = divceil(n, nTasks);
  var A:[0..#n] int;
  // Create #cores tasks
  coforall taskNum in 0..#nTasks {
    var start = nPerTask * taskNum;
    var end = start + nPerTask - 1;
    if end >= n then
      end = n-1;

    // Each task computes nPerTask random numbers
    var rng = createRandomStream(eltType=int, seed=seed);
    // Each task generates the same random values as in a serial program
    // (skipping ahead / advancing the RNG past values it would have made)
    rng.skipToNth(start);
    for i in start..end {
      A[i] = rng.getNext(min=minimum, max=maximum);
    }
  }
  return A;
}

proc main() {
  var mins = [0, 1, 2, 1024, 1 << 33, 1 << 60];
  var maxs:[0..63] int;
  for i in 0..63 {
    maxs[i] = 1 << i;
  }

  for minimum in mins {
    for maximum in maxs {
      if minimum > maximum then
        continue;

      var serialA = createRandomArraySerial(minimum, maximum);

      // Try varying between 2 and 70 tasks
      for nTasks in 2..70 by 6 {
        var parallelA = createRandomArrayParallel(nTasks, minimum, maximum);

        // Make sure they have the same values.
        for i in 0..#n {
          assert(minimum <= serialA[i] && serialA[i] <= maximum);
          if serialA[i] != parallelA[i] {
            writeln("MISATCH! nTasks=", nTasks,
                    " min=", minimum, " max=", maximum);
            return -1;
          }
        }
      }
    }
  }

  writeln("OK");
  return 0;
}
