
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

proc main() {
  var r : R;
  println(r.foo);
  println(r.wrapper());
  println(r.implicit());
  println(bar);
}
