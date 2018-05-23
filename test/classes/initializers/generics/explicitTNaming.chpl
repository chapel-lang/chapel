class Foo {
  type t;

  proc init(type t) {
    this.t = t;
  }
}

var foo = new borrowed Foo(t=int);
writeln(foo.type: string);
