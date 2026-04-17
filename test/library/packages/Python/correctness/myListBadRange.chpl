use Python;

var pyCode = """
def getList():
  return ['a', 'b', 'd', 'c']
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.createModule(pyCode);
  var getListFunc = pyCodeModule.get('getList');

  var lst = getListFunc(owned PyList);
  lst.get(0..3 by 2); // compilation error, strides aren't allowed
}
