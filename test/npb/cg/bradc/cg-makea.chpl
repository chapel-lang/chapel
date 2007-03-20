module CGMakeA {

  use Random;

  iterator makea(type elemType, n, nonzer, rcond, shift) {
    var v: [1..nonzer+1] elemType, // BLC: insert domains? or grow as necessary?
        iv: [1..nonzer+1] int;

    var size = 1.0;
    const ratio = rcond ** (1.0 / n);

    var randStr = RandomStream(314159265);

    for iouter in 1..n {
      var nzv = nonzer;

      sprnvc(elemType, n, nzv, v, iv, randStr);
      vecset(v, iv, nzv, iouter, 0.50);

      // BLC: replace with zippered loop over iv or iv(1..nzv)?
      for ivelt in 1..nzv {
        const jcol = iv(ivelt),
              scale = size * v(ivelt);

        // BLC: replace with zippered loop over iv or iv(1..nzv)?
        for ivelt1 in 1..nzv {
          const irow = iv(ivelt1);

          yield ((irow, jcol), v(ivelt1)*scale);
        }
      }
      size *= ratio;
    }

    for i in 1..n {
      yield ((i, i), rcond - shift);
    }
  }


  def sprnvc(type elemType, n, nz, v, iv, randStr) {
    var nn1 = 1;
    while (nn1 < n) do nn1 *= 2;

    var indices: domain(int);

    for nzv in 1..nz {
      var vecelt: elemType, 
          ind: int;

      do {
        vecelt = randStr.getNext();
        ind = (randStr.getNext() * nn1):int + 1;
      } while (ind > n || indices.member?(ind));

      v(nzv) = vecelt;
      iv(nzv) = ind;
      indices += ind;
    }
  }


  def vecset(v, iv, inout nzv, i, val) {
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
