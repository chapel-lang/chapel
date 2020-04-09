class Foo {
  var field: int;

  proc init(numPrints: int) {
    field = numPrints;
    forall i in 1..numPrints {
      writeln(numPrints);
    }
  }
}

var foo = new unmanaged Foo(5);
delete foo;
