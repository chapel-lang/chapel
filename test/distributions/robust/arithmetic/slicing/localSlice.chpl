use driver_arrays;

var _cur = 1;
proc next() {
  var ret = _cur;
  _cur += 1;
  return ret;
}

proc test(A : []) {
  for e in A do e = next();

  for loc in Locales do on loc {
    writeln(here.id, ": ", A.localSlice[A.localSubdomain()]);
    writeln(here.id, ": ", A[local]);

    // count the number of elements in the local slice and negate the values.
    var count = 0;
    for a in A[local] {
      assert(a > 0);
      a *= -1;
      count += 1;
    }
    {
      // make sure a standalone iteration works
      var temp : atomic int = 0;
      forall a in A[local] {
        assert(a < 0);
        temp.add(1);
      }
      assert(count == temp.read());
    }
    // ensure the number of elements is the same and that the values are all
    // negative.
    {
      var temp = 0;
      for a in A.localSlice[A.localSubdomain()] {
        assert(a < 0);
        a *= -1;
        temp += 1;
      }
      assert(count == temp);
    }

    forall (x, y) in zip(A.domain, A[local]) {
      assert(A[x] == y);
    }
  }
}

test(A1D);
test(A2D);

