// Check casting a PyDict to a Chapel map
use Python;
import Map;

var pyCode = """
def getDict():
  return dict(one=1, two=2, three=3)
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.createModule(pyCode);
  var getDictFunc = pyCodeModule.get('getDict');

  var d = getDictFunc(owned PyDict);
  writeln("dictionary size ", d.size);
  var m = d: Map.map(string, int);
  writeln("map size ", m.size);
  writeln("map[one] ", m["one"]);
  writeln("map[two] ", m["two"]);
  writeln("map[three] ", m["three"]);

  writeln("Checking Value version");
  var d2 = getDictFunc(owned Value);
  var m2 = d2: Map.map(string, int);
  writeln("map size ", m2.size);
  writeln("map[one] ", m2["one"]);
  writeln("map[two] ", m2["two"]);
  writeln("map[three] ", m2["three"]);
}
