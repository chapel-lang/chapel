
proc main {
  var ARR = [555, 666];

  forall 1..3 with (in ARR) {
    ARR = [777, 888];
    writeln(ARR);
  }

  writeln(ARR);
}
