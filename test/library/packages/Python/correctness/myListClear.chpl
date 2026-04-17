use Python;
import List;

var pyCode = """
def getList():
  return [1, 2, "hi", False, 5]
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.createModule(pyCode);
  var getListFunc = pyCodeModule.get('getList');

  var lst = getListFunc(owned PyList);
  writeln("lst ", lst);
  lst.clear();
  writeln("lst.size after clear ", lst.size);
}
