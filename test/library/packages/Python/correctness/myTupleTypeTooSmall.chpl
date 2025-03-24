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
  writeln("specifying smaller tuple than was returned");
  writeln("t[0:2] (no type) ", t.get(0..1));
  // Should we catch this?  Or just let it happen?
  writeln("t[0:2] (small type) ", t.get(1*int, 0..1));
}
