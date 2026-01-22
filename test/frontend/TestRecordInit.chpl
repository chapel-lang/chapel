
use Print;

record R {
  var x: int = 4;
  var y: int = 2;
}

record DefaultInit {
  var r : R;
}

record ExplicitInit {
  var r : R;
  proc init() {
    var r : R;
    r.x = 99;
    r.y = 101;
    this.r = r;
  }
}

proc helper() {
  return "bye";
}

record SomeImplicit {
  var first : int;
  var a = "hello";
  var b = 3.14;
  var c = 42;
  var d = helper();
  var last : int;

  proc init() {
    this.first = 1;
    this.last = 100;
  }

  proc printMe() {
    println(this.first);
    println(this.a);
    println(this.b);
    println(this.c);
    println(this.d);
    println(this.last);
  }
}

proc println(r: R) {
  println(r.x);
  println(r.y);
}

proc main() {
  var r : R;
  println(r);

  var x : DefaultInit;
  println(x.r);

  var y : ExplicitInit;
  println(y.r);

  var z = new R(100, 200);
  println(z);

  var s = new SomeImplicit();
  s.printMe();
}
