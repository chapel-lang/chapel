// Tests casting from a Python tuple to a Chapel tuple
use Python;

var pyCode = """
def getTup():
  return (2, 3, 5, 7)

def getTupMixed():
  return ("hi", False, 11)
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.importModule('__empty__', pyCode);
  var getTupFunc = pyCodeModule.get('getTup');

  var t = getTupFunc(owned PyTuple);
  writeln(t);
  var chapelT = t: (4*int);
  writeln(chapelT);

  var getTupFunc2 = pyCodeModule.get('getTupMixed');
  var t2 = getTupFunc2(owned PyTuple);
  writeln(t2);
  var chapelT2 = t2: (string, bool, int);
  writeln(chapelT2);
}
