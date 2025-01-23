use Python;
import List;

var pyCode = """
def getList():
  return [1, 2, "hi", False, 5]
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = new Module(interp, '__empty__', pyCode);
  var getListFunc = new Function(pyCodeModule, 'getList');

  var lst = getListFunc(owned PyList);
  writeln("lst ", lst);
  writeln("size ", lst.size);
  writeln("lst[0] ", lst.getItem(int, 0));
  writeln("lst[1] ", lst.getItem(int, 1));
  writeln("lst[2] ", lst.getItem(string, 2));
  writeln("lst[lst.size-1] ", lst.getItem(int, lst.size-1));

  try {
    write("lst[lst.size] ");
    writeln(lst.getItem(int, lst.size));
  } catch e: PythonException {
    writeln("Caught exception: ", e);
  } catch e{
    writeln("Caught unknown exception: ", e);
  }

  lst.setItem(0, 100);
  writeln("lst ", lst);
}
