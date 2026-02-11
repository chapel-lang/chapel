
use Print;

record R {
  proc foo {
    return 5;
  }

  proc wrapper() {
    return this.foo;
  }

  proc implicit() {
    return foo;
  }
}

proc bar {
  return 7;
}

class C {
  var x : int;

  proc noparen {
    return 42;
  }
}

proc main() {
  var r : R;
  println(r.foo);
  println(r.wrapper());
  println(r.implicit());
  println(bar);

  var c = new unmanaged C();
  println(c.noparen);
}
