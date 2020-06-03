class Foo {
  var data: int;

  proc init(x: int) {
    this.data = x;
  }
}

// This compiles.
var goodfoo: [1..2] owned Foo?;
goodfoo[1] = new owned Foo(1);
goodfoo[2] = new owned Foo(2);
writeln(goodfoo);

// This fails to compile.
var badfoo: [1..2] owned Foo? = [new owned Foo?(1), new owned Foo?(2)];
writeln(badfoo);
