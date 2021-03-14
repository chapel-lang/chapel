class Foo {
  var s: sync int;

  proc init(val: sync int) {
    writeln("inside the initializer");
    s = val;
  }
}

var outersync: sync int;
var foo1 = new Foo(outersync);
writeln(foo1.s.isFull);
delete foo1;
