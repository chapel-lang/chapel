//
// This module defines an iterator, makea(), that generates the
// pseudo-random indices and matrix values for the NAS CG benchmark
//
module CGMakeA {

  //
  // The pseudo-random number generator itself is taken from Chapel's
  // standard library "Random" module
  //
  use Random;

  //
  // configurable rcond for the random matrix
  //
  config const rcond = 0.1;

  // 
  // Here's the iterator, parameterized by the type of element to be
  // produced, the problem size, an indication of the number of
  // nonzeroes to compute, and r condition and shift factors
  //
  iter makea(type elemType, n, nonzer, shift) {
    //
    // temporary storage space for the computation
    //
    var v: [1..nonzer+1] elemType,
        iv: [1..nonzer+1] int;

    var size = 1.0;
    const ratio = rcond ** (1.0 / n);

    //
    // create the pseudo-random stream of values
    //
    var randStr = new NPBRandomStream(314159265);

    //
    // loop over the problem size
    //
    for iouter in 1..n {
      var nzv = nonzer;

      //
      // helper routines to compute the random indices and values
      //
      sprnvc(elemType, n, nzv, v, iv, randStr);
      vecset(v, iv, nzv, iouter, 0.50);

      for ivelt in 1..nzv {
        const jcol = iv(ivelt),
              scale = size * v(ivelt);

        for ivelt1 in 1..nzv {
          const irow = iv(ivelt1);

          //
          // yield an index (using a tuple) and value
          //
          yield ((irow, jcol), v(ivelt1)*scale);
        }
      }
      size *= ratio;
    }

    delete randStr;

    //
    // yield additional values for the diagonal of the matrix
    //
    for i in 1..n {
      yield ((i, i), rcond - shift);
    }
  }


  //
  // helper routine for creating sparse entries
  //
  proc sprnvc(type elemType, n, nz, v, iv, randStr) {
    var nn1 = 1;
    while (nn1 < n) do nn1 *= 2;

    //
    // an indefinite (associative) domain to store the indices we've
    // seen this time through the loop -- serves as a set
    //
    var indices: domain(int);

    for nzv in 1..nz {
      var vecelt: elemType, 
          ind: int;

      //
      // roll random numbers until we get an index that's in-bounds
      // and that we haven't seen yet
      //
      do {
        vecelt = randStr.getNext();
        ind = (randStr.getNext() * nn1):int + 1;
      } while (ind > n || indices.contains(ind));

      //
      // store the random values
      //
      v(nzv) = vecelt;
      iv(nzv) = ind;
      indices += ind;
    }
  }


  //
  // another helper routine that, in some cases, adds another sparse
  // value
  //
  proc vecset(v, iv, inout nzv, i, val) {
    var set = false;
    for k in 1..nzv {
      if (iv(k) == i) {
        v(k) = val;
        set = true;
      }
    }
    if (!set) {
      nzv += 1;
      v(nzv) = val;
      iv(nzv) = i;
    }
  }
}
