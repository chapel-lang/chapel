use OwnedObject;

class Foo {
  var x: int;

  proc init(x: int) {
    this.x = x;
  }
}

var a = new Owned(new Foo(1));
var b = new Owned(new Foo(2));

a <=> b;

writeln(a);
writeln(b);

var c = new Shared(new Foo(3));
var d = new Shared(new Foo(4));
var copyOfD = d;
var anotherCopOfD = d;

writeln(c.pn);
writeln(d.pn);

c <=> d;

writeln(c);
writeln(d);
writeln(c.pn);
writeln(d.pn);
