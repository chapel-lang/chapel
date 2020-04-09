class R {
  type t;
  var  x:t;

  class R2 {
    type t;
    var y:t;
  }

  proc test() {
    var r2 = new unmanaged R2(t, x);

    writeln(r2.y);

    delete r2;
  }
}


var r = new unmanaged R(int, 7);

r.test();

delete r;
