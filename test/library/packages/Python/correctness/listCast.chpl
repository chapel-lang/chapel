// Test casting from a PyList to a list
use Python;
import List;

var pyCode = """
def getList():
  return [1, 2, 5, -3, -1]

def getListMixed():
  return [1, 2, "hi", False, 5]
""";

proc main() {

  var interp = new Interpreter();

  var pyCodeModule = interp.importModule('__empty__', pyCode);
  var getListFunc = pyCodeModule.get('getList');

  var lst = getListFunc(owned PyList);
  writeln("lst ", lst);
  var chapelList = lst: List.list(int);
  writeln("chapelList ", chapelList);

  // Check getting a Python list back that contains multiple types
  var getListFunc2 = pyCodeModule.get('getListMixed');
  var lst2 = getListFunc2(owned PyList);
  writeln("lst2 ", lst2);
  var chapelList2 = lst2: List.list(shared Value?);
  writeln("chapelList2 ", chapelList2);
}
