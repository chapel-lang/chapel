use Python;
import Set;

var pyCode = """
def getSet():
  return {'a', 'b', 'c'}
""";

proc testSet(s: borrowed) {
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
  writeln("=========");
}

proc main() {
  var interp = new Interpreter();

  var pyCodeModule = interp.createModule(pyCode);
  var getSetFunc = pyCodeModule.get('getSet');
  var s = getSetFunc(owned PySet);
  testSet(s);

  var set = interp.get('set');
  var s2 = set(owned PySet, ['a', 'b', 'c']);
  testSet(s2);

  // TODO: could be replaced nicely with a cast?
  var s3 = new PySet(interp,
                     interp.toPython(new Set.set(string, ['a', 'b', 'c'])));
  testSet(s3);

}
