class A {
  var x: int;
  proc getX() do return x;
}

class B : A {
  override proc getX() do return x + 1;
}
