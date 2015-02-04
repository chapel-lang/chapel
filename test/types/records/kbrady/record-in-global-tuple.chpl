module bar {
  record BadBaz {
    var x = [1];

    proc BadBaz(y) {
      x[1] = y;
    }
  }

  var tuple_failure = (new BadBaz(2367),);
}

module foo {
  use bar;

  proc main() {
    writeln(tuple_failure[1].x[1]);
  }
}
