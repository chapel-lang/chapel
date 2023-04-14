
record Foo {
  var x : 3*simpleRange; // could be generic if we fix implementation

  proc init() {
    x = (1..1, 1..2, 1..3);
  }
}

const f = new Foo();
