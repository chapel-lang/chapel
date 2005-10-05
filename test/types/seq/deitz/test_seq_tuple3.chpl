iterator foo() : (integer, integer) {
  forall i:integer in 1..10 {
    forall j:integer in 1..10 {
      yield (i, j);
    }
  }
}

writeln(foo());
