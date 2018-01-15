record Foo {
  var x: int;
  var y: bool;

  proc init(x: int) {
    this.x = x;
    this.y = true;
    super.init();
  }

  proc init() {
    x = 0;
    y = true;
    super.init();
  }

  proc init(other: Foo) {
    x = other.x;
    y = false;
    super.init();
  }
}

var one = new Foo(1);

proc *(a: Foo, b: Foo) {
  var ret = new Foo(a.x*b.x);
  return ret;
}

// Compiles and runs when I want the error to turn up
inline proc _cast(type t, const x: int) where t == Foo {
  var ret = new Foo(x);
  return ret;
}

proc reductive_sequential(n:int): Foo {
  return * reduce for i in 1..n do new Foo(i);
}

proc main() {
  writeln(reductive_sequential(0));
}
