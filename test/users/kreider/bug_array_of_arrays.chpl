/* bug: Array-of-array argument causes compiler error. */

class tst {
  type dataType;
  param ndim : int;
  var Ldata : domain(rank=1);

  proc tst(nelt : int, type dataType, param ndim) {
    Ldata = 1..nelt;
  }

  proc run_test() {
    var sortind : [1..ndim][Ldata] int;

    for c in 1..ndim {
      for i in Ldata do sortind(c)(i) = i * ndim + c;
    }

    pass_arrayofarrays(sortind);
  }

  proc pass_arrayofarrays(d : [][] int) {
    /* Should be 11. */
    writeln("val at 2,3 is ", sortind(2)(3));
  }
}

var t = new tst(5, int, 3);
t.run_test();


