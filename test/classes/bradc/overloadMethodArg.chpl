class C {
  def foo(foo:int) {
    if (foo < 0) {
      writeln("negative");
    } else {
      writeln("positive");
    }
  }

  def bar(param bar) {
    if (bar < 0) {
      writeln("negative");
    } else {
      writeln("positive");
    }
  }

  def baz(foo:int, bar: int) {
    if (foo < bar) {
      writeln("less");
    } else {
      writeln("greater");
    }
  }
}

var c = new C();
c.foo(2);
c.bar(3);
c.baz(4,5);

