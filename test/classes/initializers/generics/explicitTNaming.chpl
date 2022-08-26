class Foo {
  type t;

  proc init(type t) {
    this.t = t;
  }
}

var foo = (new owned Foo(t=int)).borrow();
writeln(foo.type: string);
