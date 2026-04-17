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

  var chapelList = new List.list(int);
  chapelList.pushBack(11);
  lst.extend(chapelList); // send a Chapel list
  writeln("lst ", lst);

  var arr = [true, true, false];
  lst.extend(arr); // send an array
  writeln("lst ", lst);
}
