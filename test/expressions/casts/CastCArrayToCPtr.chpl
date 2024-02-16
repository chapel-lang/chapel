use CTypes;

record Foo {
  var xs : c_array(int, 5);
}

proc bar(x : c_ptr(int)) {
  x[0] = 8;
}

proc part1() {
  var raw : c_array(int, 5);
  writeln(raw);
  bar(raw);
  writeln(raw);
}

proc part2() {
  var foo = new Foo();
  writeln(foo.xs);
  bar(foo.xs);
  writeln(foo.xs);
}

proc justOneCast() {
  var foo = new Foo();
  bar(foo.xs);
}

proc main() {
  part1();
  part2();
  justOneCast();
}
