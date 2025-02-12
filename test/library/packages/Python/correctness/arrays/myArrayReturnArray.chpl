use Python;

var pythonCode = """
def returnSelf(arr):
  return arr
""";

proc main() {
  var arr: [1..10] int = 1..10;

  var interp = new Interpreter();

  var pyArr = new Array(interp, arr);
  var pyCode = interp.importModule('__empty__', pythonCode);
  var func = pyCode.get('returnSelf');
  var newArr = func(owned Array, pyArr);
  writeln("from chapel: ", newArr);
}
