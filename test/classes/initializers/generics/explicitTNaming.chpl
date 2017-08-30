class Foo {
  type t;

  proc init(type t) {
    this.t = t;
    super.init();
  }
}

var foo = new Foo(t=int);
writeln(foo.type: string);
delete foo;
