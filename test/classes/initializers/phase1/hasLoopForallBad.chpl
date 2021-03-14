class Foo {
  var field: int;

  proc init(numPrints: int) {
    // This is a race,
    // you aren't allow to initialize fields in parallel statements,
    // you aren't allow to initialize fields in loops.
    forall i in 1..numPrints {
      field = i;
    }
  }
}

var foo = new shared Foo(5);
writeln(foo);
