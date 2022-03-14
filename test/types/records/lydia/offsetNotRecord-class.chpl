use CTypes;
class Foo {
  var x: int;
}


writeln(c_offsetof(Foo, "x"));
