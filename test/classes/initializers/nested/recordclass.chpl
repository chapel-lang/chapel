pragma "use default init"
record R {
  type t;
  var x:t;

  pragma "use default init"
  class R2 {
    type t;
    var y:t;
  }

  proc test() {
    var r2 = new R2(t, x);

    writeln(r2.y);

    delete r2;
  }
}


var r = new R(int, 7);

r.test();

