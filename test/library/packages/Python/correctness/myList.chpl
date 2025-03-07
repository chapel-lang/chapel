use Python;
import List;

var pyCode = """
def getList():
  return [1, 2, "hi", False, 5]
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.importModule('__empty__', pyCode);
  var getListFunc = pyCodeModule.get('getList');

  var lst = getListFunc(owned PyList);
  writeln("lst ", lst);
  writeln("size ", lst.size);
  writeln("lst[0] ", lst.get(int, 0));
  writeln("lst[1] ", lst.get(int, 1));
  writeln("lst[2] ", lst.get(string, 2));
  writeln("lst[lst.size-1] ", lst.get(int, lst.size-1));
  writeln("lst[-3] ", lst.get(owned Value, -3));
  writeln("lst[-3] ", lst.get(-3));

  try {
    write("lst[lst.size] ");
    writeln(lst.get(int, lst.size));
  } catch e: PythonException {
    writeln("Caught exception: ", e);
  } catch e{
    writeln("Caught unknown exception: ", e);
  }

  lst.set(0, 100);
  writeln("lst ", lst);
}
