use Random;
use Time;

config const debug = false;


proc getRealRandoms(method:int) {
  var A:[1..100] real;

  if method == 0 {
    var R = new owned RandomStream(real);

    if debug then
      writeln("method 0, seed ", R.seed);

    for a in A do
      a = R.getNext();
  }

  if method == 1 {
    var R = createRandomStream(eltType=real);

    if debug then
      writeln("method ", method, ", seed ", R.seed);

    for a in A do
      a = R.getNext();
  }


  if method == 2 {
    var R = createRandomStream(eltType=real, algorithm=RNG.PCG);

    if debug then
      writeln("method ", method, ", seed ", R.seed);

    for a in A do
      a = R.getNext();
  }

  if method == 3 {
    var R = new owned NPBRandomStream(real);

    if debug then
      writeln("method ", method, ", seed ", R.seed);

    for a in A do
      a = R.getNext();
  }

  if method == 4 {
    var R = createRandomStream(eltType=real, algorithm=RNG.NPB);

    if debug then
      writeln("method ", method, ", seed ", R.seed);

    for a in A do
      a = R.getNext();
  }

  return A;
}

proc getUintRandoms(method:int) {
  var A:[1..100] uint;

  if method == 0 {
    var R = new owned RandomStream(uint);

    if debug then
      writeln("method ", method, ", seed ", R.seed);

    for a in A do
      a = R.getNext();
  }

  if method == 1 {
    var R = createRandomStream(eltType=uint);

    if debug then
      writeln("method ", method, ", seed ", R.seed);

    for a in A do
      a = R.getNext();
  }

  if method == 2 {
    var R = createRandomStream(eltType=uint, algorithm=RNG.PCG);

    if debug then
      writeln("method ", method, ", seed ", R.seed);

    for a in A do
      a = R.getNext();
  }

  return A;
}


proc waitForNextSeed() {
  // Wait for the time-based seed generator to give us
  // a different value. The loop guards against
  // under-sleeping.
  var curSeed = SeedGenerator.oddCurrentTime;
  var newSeed = curSeed;
  do {
    sleep(2, TimeUnits.milliseconds);
    newSeed = SeedGenerator.oddCurrentTime;
  } while (newSeed == curSeed);
}

for i in 0..4 {
  // Get one random array
  var A = getRealRandoms(i);
  // Wait
  waitForNextSeed();
  // Get another, now the time should be different ->
  // the seed should be different
  var B = getRealRandoms(i);
  // Check that these are not the same
  var equal = && reduce (A == B);
  if equal {
    writeln("Error with time-based seed with real i=  ", i);
    writeln("A = ", A);
    writeln("B = ", B);
  }
}

for i in 0..2 {
  // Get one random array
  var A = getUintRandoms(i);
  // Wait
  waitForNextSeed();
  // Get another, now the time should be different ->
  // the seed should be different
  var B = getUintRandoms(i);
  // Check that these are not the same
  var equal = && reduce (A == B);
  if equal {
    writeln("Error with time-based seed with uint i=", i);
    writeln("A = ", A);
    writeln("B = ", B);
  }
}


