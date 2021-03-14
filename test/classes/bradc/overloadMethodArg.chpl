class C {
  proc foo(foo:int) {
    if (foo < 0) {
      writeln("negative");
    } else {
      writeln("positive");
    }
  }

  proc bar(param bar) {
    if (bar < 0) {
      writeln("negative");
    } else {
      writeln("positive");
    }
  }

  proc baz(foo:int, bar: int) {
    if (foo < bar) {
      writeln("less");
    } else {
      writeln("greater");
    }
  }
}

var c = new unmanaged C();
c.foo(2);
c.bar(3);
c.baz(4,5);
delete c;
