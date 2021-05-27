deprecated "Don't use Foo"
record Foo {
  var x: int;
}

deprecated "Don't use bar either"
proc bar() {
  var f: Foo; // I would argue this shouldn't trigger the warning
  writeln(f);
}

proc main() {
  bar();  // Should trigger the warning
}