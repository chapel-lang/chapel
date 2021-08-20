
record Foo {
  var x : 3*range(stridable=false);

  proc init() {
    x = (1..1, 1..2, 1..3);
  }
}

const f = new Foo();
