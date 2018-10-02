class Foo {
  var x: int;

  proc init(x: int) {
    this.x = x;
  }
}

var a = new owned Foo(1);
var b = new owned Foo(2);

a <=> b;

writeln(a);
writeln(b);

var c = new shared Foo(3);
var d = new shared Foo(4);
var copyOfD = d;
var anotherCopOfD = d;

writeln(c.chpl_pn);
writeln(d.chpl_pn);

c <=> d;

writeln(c);
writeln(d);
writeln(c.chpl_pn);
writeln(d.chpl_pn);
