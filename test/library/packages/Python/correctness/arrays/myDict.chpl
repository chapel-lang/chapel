use Python;
import Map;

var pyCode = """
def getDict():
  return dict(one=1, two=2, three=3)
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.importModule('__empty__', pyCode);
  var getDictFunc = pyCodeModule.get('getDict');

  var d = getDictFunc(owned PyDict);
  writeln("d ", d);
  writeln("size ", d.size);
  writeln("get one ", d.get("one"));
  writeln("get two ", d.get(int, "two"));
  writeln("get three ", d.get("three"));
  writeln("contains one ", d.contains("one"));
  writeln("contains two ", d.contains("two"));
  writeln("contains three ", d.contains("three"));
  writeln("contains four ", d.contains("four"));

  d.set("four", 4);
  writeln("get four after set four ", d.get("four"));

  var d2 = d.copy();
  writeln("d2 size ", d2.size);

  d.del("three");
  writeln("size after delete of three ", d.size);

  d.clear();
  writeln("size after clear ", d.size);
  writeln("d2 size after d1 clear ", d2.size);

  try {
    d.del("five"); // Try deleting something not in the dict
  } catch e: PythonException {
    writeln("Caught exception: ", e);
  } catch e {
    writeln("Caught unknown exception: ", e);
  }
}
