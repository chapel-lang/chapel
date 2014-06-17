class R {
  type t;
  var x:t;
  record R2 {
    var y:t;
  }
  proc test() {
    var r2:R2;
    writeln(r2.y);
  }
}


var r = new R(int, 7);

r.test();

