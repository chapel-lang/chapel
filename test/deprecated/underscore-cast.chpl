record A {
  var x: int;
}

record B {
  var y: int;
}

proc _cast(type t: A, from: B) {
  return new A(from.y);
}

proc main() {
  var a = new A(1);
  var b = new B(2);
  a = b:A;
  writeln(a);
}
