use Random;

{
  var rng = makeRandomStream(seed=17, parSafe=false,
                             eltType=real(64), algorithm=RNG.PCG);

  var v1 = rng.getNext();
  writeln(v1.type:string);

  var v2 = rng.getNext(uint(8));
  writeln(v2.type:string);

  var v3 = rng.getNext(uint(64));
  writeln(v3.type:string);

  delete rng;
}

// Test stepping for uint
{
  var v2: uint(64);

  {
    var rng = makeRandomStream(seed=17, parSafe=false,
                               eltType=uint(64), algorithm=RNG.PCG);

    var v1  = rng.getNext();

    v2 = rng.getNext();

    delete rng;
  }

  {
    var rng = makeRandomStream(seed=17, parSafe=false,
                               eltType=uint(64), algorithm=RNG.PCG);

    var v1     = rng.getNext(uint(8));
    var got_v2 = rng.getNext();

    assert(v2 == got_v2);

    delete rng;
  }
}

// Test stepping for real
{
  var v2: real;

  {
    var rng = makeRandomStream(seed=17, parSafe=false,
                               eltType=real(64), algorithm=RNG.PCG);

    var v1 = rng.getNext();

    v2 = rng.getNext();

    delete rng;
  }

  {
    var rng = makeRandomStream(seed=17, parSafe=false,
                               eltType=real(64), algorithm=RNG.PCG);

    var v1 = rng.getNext(uint(8));
    var got_v2 = rng.getNext();

    assert(v2 == got_v2);

    delete rng;
  }
}


// Test stepping for complex
{
  var v2: complex;

  {
    var rng = makeRandomStream(seed=17, parSafe=false,
                               eltType=complex(64), algorithm=RNG.PCG);

    var v1 = rng.getNext();

    v2 = rng.getNext();

    delete rng;
  }

  {
    var rng = makeRandomStream(seed=17, parSafe=false,
                               eltType=complex(64), algorithm=RNG.PCG);

    var v1     = rng.getNext(uint(8));
    var got_v2 = rng.getNext();

    assert(v2 == got_v2);

    delete rng;
  }
}

