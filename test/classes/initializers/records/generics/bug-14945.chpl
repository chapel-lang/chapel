record Foo {
  var v;
  proc init(v = 3) {
    this.v = v;
  }
}

var foo1: Foo(bool); // should be an error since Foo(bool).init() sets v=3
writeln(foo1.type:string);
