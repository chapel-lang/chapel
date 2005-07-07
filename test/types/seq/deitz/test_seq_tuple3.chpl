iterator foo() {
  forall i in 1..10 {
    forall j in 1..10 {
      yield (i, j);
    }
  }
}

writeln(foo());
