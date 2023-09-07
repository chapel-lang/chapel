foreach i in 1..10 {
  writeln(i);
}

{ // just to be sure
  forall i in 1..10 {
    writeln(i);
  }
}

{ // just to be sure
  use BlockDist;

  var D = blockDist.createDomain(1..10);
  forall i in D {
    writeln(i);
  }
}
