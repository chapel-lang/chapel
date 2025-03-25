use Python;

var pyCode = """
def getTup():
  return (2, 3, 5, 7)

def getTup2():
  return (11,)
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.createModule(pyCode);
  var getTupFunc = pyCodeModule.get('getTup');

  var t = getTupFunc(owned PyTuple);
  writeln("size ", t.size);
  writeln("t[0] ", t.get(int, 0));
  writeln("t[1] ", t.get(1));
  writeln("t[2] ", t.get(int, 2));
  writeln("t[3] ", t.get(int, 3));

  // check contains
  writeln("5 in t: ", t.contains(5));
  writeln("8 in t: ", t.contains(8));

  // get a slice of the tuple
  writeln("t[0:2] ", t.get(0..1));
  writeln("t[1:2] ", t.get(1..1));
  writeln("t[2:5] ", t.get(2..5));
  writeln("t[-3:2] ", t.get(-3..1));
  writeln("t[-3:5] ", t.get(-3..5));

  // get a slice with an empty range
  writeln("t[1:1] ", t.get(1..0));

  // slice missing at least one bounds
  writeln("t[2:] ", t.get(2..));
  writeln("t[:3] ", t.get(..2));
  writeln("t[:] ", t.get(..));

  // get slice with range and type argument
  writeln("checking explicit type for slice overload");
  writeln("t[0:2] ", t.get(owned PyTuple, 0..1));
  writeln("t[2:5] ", t.get(2*int, 2..5));
  // Note: calling non-tuple methods on the result of this does not work and it
  // does not print as a list would.  But you can still make the call
  writeln("t[0:2] ", t.get(owned PyList, 0..1));

  // Error cases
  try {
    // get(idx) when idx is negative
    writeln("checking negative idx");
    var x = t.get(int, -3);
  } catch e: PythonException {
    writeln("Caught exception: ", e);
  } catch e {
    writeln("Caught unknown exception: ", e);
  }

  try {
    // get(idx) when idx is beyond last index
    writeln("checking out of bounds idx");
    var x = t.get(int, 7);
  } catch e: PythonException {
    writeln("Caught exception: ", e);
  } catch e {
    writeln("Caught unknown exception: ", e);
  }

  try {
    // get(int, bounds) should not work
    writeln("checking trying to convert slice to an int");
    var x = t.get(int, 1..2);
  } catch e: PythonException {
    writeln("Caught exception: ", e);
  } catch e {
    writeln("Caught unknown exception: ", e);
  }

  try {
    // get specifying a type that is longer than the slice requested
    writeln("checking trying to slice into too large a tuple");
    var x = t.get(3*int, 0..1);
  } catch e: ChapelException {
    writeln("Caught exception: ", e);
  } catch e {
    writeln("Caught unknown exception: ", e);
  }

  // Double check 1 element tuple
  var getTup2Func = pyCodeModule.get('getTup2');
  var t2 = getTup2Func(owned PyTuple);
  writeln(t2);
}
