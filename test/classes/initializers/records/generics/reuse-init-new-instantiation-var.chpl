// This test exercises calling the same initializer twice when a record has a
// generic var field, but using different types for the generic argument

record Foo {
  var x;

  proc init() {
    var default: this.type.x;
    this.x = default;
  }

  proc init(x) {
    this.x = x;
  }
}

var foo1: Foo(int);
var foo2: Foo(bool);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
