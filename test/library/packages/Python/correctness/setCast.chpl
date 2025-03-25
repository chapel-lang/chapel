// Test casting a Python set to a Chapel set
use Python;
import Set;

var pyCode = """
def getSet():
  return {'a', 'b', 'c'}
""";

proc main() {
  var interp = new Interpreter();

  var pyCodeModule = interp.importModule('__empty__', pyCode);
  var getSetFunc = pyCodeModule.get('getSet');

  var s = getSetFunc(owned PySet);
  writeln("pySet size ", s.size);
  var chapelSet = s: Set.set(string);
  writeln("chapel set size ", chapelSet.size);

  writeln("chapelSet contains a ", chapelSet.contains("a"));
  writeln("chapelSet contains b ", chapelSet.contains("b"));
  writeln("chapelSet contains c ", chapelSet.contains("c"));
}
