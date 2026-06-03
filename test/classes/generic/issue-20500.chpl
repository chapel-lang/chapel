class Thing {}

class Foo {
  var x : owned;
}

class Bar: Foo(?) {}

var myBar = new Bar(new Thing());
