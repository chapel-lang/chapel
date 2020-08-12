class Foo {
  var x$: sync int = 5;

  // TODO: compiler should generate this (formal without sync)
  proc init(x: int = 5) {
    this.x$ = x;
  }
}

var foo = new unmanaged Foo();
writeln(foo.x$.readFF());
delete foo;
