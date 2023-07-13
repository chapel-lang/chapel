record Foo {
  var x: [0..3][0..3] sync string;
  var otherField: int;

  proc init(val: int) {
    otherField = val;
  }
  proc init=(other: Foo) {
    this.x = other.x; // should this work?
    this.otherField = other.otherField;
  }
}

proc main() {
  var f = new Foo(3);
  writeln(f.otherField);
}
