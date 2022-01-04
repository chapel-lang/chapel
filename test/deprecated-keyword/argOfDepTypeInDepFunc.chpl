deprecated "Don't use Foo"
record Foo {
  var x: int;
}

deprecated "Don't use bar either"
proc bar(f: Foo) { // I would argue this shouldn't trigger the warning
  writeln(f);
}

proc main() {
  var f = new Foo(3); // Should trigger the warning
  bar(f);             // Should trigger the warning
}
