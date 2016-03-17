use Random;

config const verbose = false;

var seed = 42;

var histo:[0..255] int;

// Check we produce every random byte
{
  var rs = makeRandomStream(seed = seed, parSafe=false,
                            eltType = uint(8), algorithm=RNG.PCG);

  for i in 1..1000000 {
    var got = rs.getNext();
    histo[got] += 1;
  }

  for (h,i) in zip(histo,0..) {
    assert(h > 0);
    if verbose then writef("% 3i: %i\n", i, h);
  }
}


histo = 0;

// Check we produce every random byte with sub-range
{
  var rs = makeRandomStream(seed = seed, parSafe=false,
                            eltType = uint(8), algorithm=RNG.PCG);

  for i in 1..1000000 {
    var got = rs.getNext(5, 20);
    histo[got] += 1;
  }

  for (h,i) in zip(histo,0..) {
    if 5 <= i && i <= 20 then assert(h > 0);
    else assert(h == 0);
    if verbose then writef("% 3i: %i\n", i, h);
  }
}

histo = 0;

// Check we produce every random byte with sub-range
{
  var rs = makeRandomStream(seed = seed, parSafe=false,
                            eltType = uint(64), algorithm=RNG.PCG);

  for i in 1..1000000 {
    var got = rs.getNext(5, 20):int;
    histo[got] += 1;
  }

  for (h,i) in zip(histo,0..) {
    if 5 <= i && i <= 20 then assert(h > 0);
    else assert(h == 0);
    if verbose then writef("% 3i: %i\n", i, h);
  }
}


