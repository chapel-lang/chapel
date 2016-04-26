
proc main {
  const DOM = {555..666};
  const ARR = [555, 666];

  forall 1..3 with (in DOM, in ARR) {
    DOM = {777..888};
    ARR = [777, 888];
    writeln(ARR, " ", DOM);
  }

  writeln("done ", ARR, " ", DOM);
}
