
proc main {
  var DOM = {555..666};

  forall 1..3 with (in DOM) {
    DOM = {777..888};
    writeln(DOM);
  }

  writeln(DOM);
}
