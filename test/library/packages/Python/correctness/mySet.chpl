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
  // writeln("s ", s); // Too inconsistent, it's a set
  writeln("size ", s.size);
  s.add('d');
  writeln("size after add ", s.size);
  writeln("contains 'a' ", s.contains("a"));
  writeln("contains 'b' ", s.contains("b"));
  writeln("contains 'c' ", s.contains("c"));
  writeln("contains 'd' ", s.contains("d"));

  s.discard('b');
  writeln("size after discarding 'b' ", s.size);
  writeln("contains 'b' ", s.contains("b"));

  var poppedVal = s.pop();
  writeln("contains popped value ", s.contains(poppedVal));
  writeln("size after pop ", s.size);

  var poppedVal2 = s.pop(string);
  writeln("contains popped value ", s.contains(poppedVal2));
  writeln("size after pop ", s.size);

  s.clear();
  writeln("size after clear ", s.size);
}
