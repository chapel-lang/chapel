class R {
  type t;
  var x:t;

  record A {
    type t;
    var y:t;
  }

  proc test() {
    var a:A(t);
    var b:A(int) = new A(t, x);
    var c:A(int) = new A(t, y=x);

    writeln(a.y);
    writeln(b.y);
    writeln(c.y);
  }
}


var r = new owned R(int, 7);

r.test();
