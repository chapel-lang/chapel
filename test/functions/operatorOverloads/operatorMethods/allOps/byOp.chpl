// Test defining the 'by' operator as a method
record Foo {
  var x: int;

  operator by(r: range(?i), count: Foo) {
    writeln("In Foo.by");
    return r by count.x;
  }
}

proc main() {
  var r1 = 0..6;
  var r2 = 2..9;
  var foo = new Foo(3);
  writeln(r1 by foo);
  writeln(r2 by foo);
}
