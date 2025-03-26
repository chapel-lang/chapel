// Test casting a Python set to a Chapel set
use Python;
import Set;

var pyCode = """
def getSet():
  return {'a', 'b', 'c'}
""";

proc main() {
  var interp = new Interpreter();

  var pyCodeModule = interp.createModule(pyCode);
  var getSetFunc = pyCodeModule.get('getSet');

  var s = getSetFunc(owned PySet);
  writeln("pySet size ", s.size);
  var chapelSet = s: Set.set(string);
  writeln("chapel set size ", chapelSet.size);

  writeln("chapelSet contains a ", chapelSet.contains("a"));
  writeln("chapelSet contains b ", chapelSet.contains("b"));
  writeln("chapelSet contains c ", chapelSet.contains("c"));

  writeln("checking Value version");
  var s2 = getSetFunc(owned Value);
  var chapelSet2 = s2: Set.set(string);
  writeln("chapel set2 size ", chapelSet2.size);

  writeln("chapelSet2 contains a ", chapelSet2.contains("a"));
  writeln("chapelSet2 contains b ", chapelSet2.contains("b"));
  writeln("chapelSet2 contains c ", chapelSet2.contains("c"));
}
