class myouter {
  var a = 3;

  class inner {
    var b = 2;

    proc bar() {
      writeln("inner.b is: ", b);
    }
  }

  proc foo() {
    var inside_inner = new unmanaged inner();

    writeln("outer.a is: ", a);
    inside_inner.bar();

    delete inside_inner;
  }
}

proc main() {
  var outside: unmanaged myouter = new unmanaged myouter();

  outside.foo();

  delete outside;
}
