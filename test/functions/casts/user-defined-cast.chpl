record A {
  var x: int;
}

record B {
  var y: int;
}

operator : (from: B, type t: A) {
  return new A(from.y);
}

proc main() {
  var a = new A(1);
  var b = new B(2);
  a = b:A;
  writeln(a);
}
