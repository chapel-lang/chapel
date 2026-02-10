record thing { type genericField; }

class Foo {
  var x : thing(?);
}

class Bar: Foo(?) {}

var myBar = new Bar(new thing(int));
