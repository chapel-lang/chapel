use Python;

var pyCode = """
def getTup():
  return ('a', 'b', 'd', 'c')
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.createModule(pyCode);
  var getTupFunc = pyCodeModule.get('getTup');

  var t = getTupFunc(owned PyTuple);
  t.get(0..3 by 2); // compilation error, strides aren't allowed
}
