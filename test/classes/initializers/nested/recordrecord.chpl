record R {
  type t;
  var x:t;
  record R2 {
    type t;
    var y:t;
  }
  proc test() {
    var a:R2(t);
    var b:R2(int) = new R2(t, x);
    var c:R2(int) = new R2(t, y=x);

    writeln(a.y);
    writeln(b.y);
    writeln(c.y);
  }
}


var r = new R(int, 7);

r.test();

