// Test defining the count operator as a method
record Foo {
  var x: int;

  operator #(r: range(?i), count: Foo) {
    writeln("In Foo.#");
    return r # count.x;
  }
}

proc main() {
  var r1 = 0..6;
  var r2 = 2..9;
  var foo = new Foo(3);
  writeln(r1 # foo);
  writeln(r2 # foo);
}
