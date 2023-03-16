class Foo {
  type t;

  proc init(type t) {
    this.t = t;
  }
}

var ownFoo = new owned Foo(t=int);
var foo = ownFoo.borrow();
writeln(foo.type: string);
