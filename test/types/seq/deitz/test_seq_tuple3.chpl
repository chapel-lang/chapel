iterator foo() : (int, int) {
  forall i:int in 1..10 {
    forall j:int in 1..10 {
      yield (i, j);
    }
  }
}

writeln( _to_seq( foo()));
