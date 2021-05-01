// Test defining the 'align' operator as a method
record Foo {
  var x: int;

  operator align(r: range(?i), count: Foo) {
    writeln("In Foo.align");
    return r align count.x;
  }
}

proc main() {
  var r1 = 0..6;
  var r2 = 2..9;
  var foo = new Foo(3);

  var r1b = r1 by 4 align foo;
  writeln(r1b);
  for i in r1b {
    writeln(i);
  }

  var r2b = r2 by 4 align foo;
  writeln(r2b);
  for i in r2b {
    writeln(i);
  }
}
