class Foo {
  var field: int;

  proc init(numPrints: int) {
    field = numPrints;
    forall i in 1..numPrints {
      writeln(numPrints);
    }
    super.init();
  }
}

var foo = new Foo(5);
delete foo;
