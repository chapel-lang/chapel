record A {
  type t;
  var x: t;

  proc init(x) {
    this.t = x.type;
    this.x = x;
  }

  proc init(a:A) { // copy initializer to avoid confusion with the above
    this.t = a.t;
    this.x = a.x;
  }
}

var a: A(int) = new A(42.0);
writeln(a);
