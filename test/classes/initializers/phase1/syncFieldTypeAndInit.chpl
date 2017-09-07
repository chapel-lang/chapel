class Foo {
  var s: sync int = 4;

  proc init(val) {
    s = val;
    super.init();
  }
}

var foo1 = new Foo(3);
writeln(foo1.s.readFF());
delete foo1;
