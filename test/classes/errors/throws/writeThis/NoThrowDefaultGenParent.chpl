class A {
  var x: int = 0;
}

class A1: A {
  var y: int = 0;

  proc writeThis(ch) throws {
    ch <~> "Hello from class A1!";
  }
}

proc main() {
  var a: A = new A();
  var b: A1 = new A1();
  var c: A = new A1();

  writeln(a);
  writeln(b);
  writeln(c);
}
