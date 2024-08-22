class Foo {
  type t;
  var next: t;
}

var o = new owned Foo(owned Foo);
