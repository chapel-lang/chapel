use Python;


record R { var x: int; }

proc main() {
  var interp = new Interpreter();

  try {
    var v = new Value(interp, new R(1));
    writeln("Should have thrown an exception");
  } catch e {
    writeln("Caught exception: ", e);
  }

  try {
    var v = new Value(interp, 10);
    v.value(R);
    writeln("Should have thrown an exception");
  } catch e {
    writeln("Caught exception: ", e);
  }
}


