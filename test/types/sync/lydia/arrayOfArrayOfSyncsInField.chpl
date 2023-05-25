record Foo {
  var x: [0..3][0..3] sync string;
  var otherField: int;

  proc init(val: int) {
    otherField = val;
  }
}

proc main() {
  var f = new Foo(3);
  writeln(f.otherField);
}
