class Foo {
  var D: domain(1);
  var A: [D] real = [i in D] i;

  proc init(dom) {
    D = dom;
  }
}

var foo = new unmanaged Foo(1..10);
writeln(foo);
delete foo;
