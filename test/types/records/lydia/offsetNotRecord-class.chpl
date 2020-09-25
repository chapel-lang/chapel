use CPtr;
class Foo {
  var x: int;
}


writeln(c_offsetof(Foo, "x"));
