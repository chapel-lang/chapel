class R {
  type t;
  var  x:t;

  class R2 {
    var y:t;
  }

  proc test() {
    var r2 = new R2(x);

    writeln(r2.y);

    delete r2;
  }
}


var r = new R(int, 7);

r.test();

delete r;
