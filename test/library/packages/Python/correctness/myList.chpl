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
  writeln("size ", lst.size);
  writeln("lst[0] ", lst.get(int, 0));
  writeln("lst[1] ", lst.get(int, 1));
  writeln("lst[2] ", lst.get(string, 2));
  writeln("lst[lst.size-1] ", lst.get(int, lst.size-1));
  writeln("lst[-3] ", lst.get(owned Value, -3));
  writeln("lst[-3] ", lst.get(-3));

  writeln("lst[2] ", lst.get(2));
  lst.insert(2, -4);
  writeln("lst[2] after insert ", lst.get(2));

  lst.append("goodbye");
  writeln("lst post append: ", lst);

  var lstSlice = lst.get(1..3);
  writeln("lst[1:4] ", lstSlice);

  var lstSlice2 = lst.get(5..);
  writeln("lst[5:] ", lstSlice2);
  writeln("lst[:2] ", lst.get(..1));
  writeln("lst[:] ", lst.get(..));

  try {
    write("lst[lst.size] ");
    writeln(lst.get(int, lst.size));
  } catch e: PythonException {
    writeln("Caught exception: ", e);
  } catch e{
    writeln("Caught unknown exception: ", e);
  }

  try {
    // get(int, bounds) should not work
    writeln("checking trying to convert slice to an int");
    var x = lst.get(int, 1..2);
  } catch e: PythonException {
    writeln("Caught exception: ", e);
  } catch e {
    writeln("Caught unknown exception: ", e);
  }

  lst.set(0, 100);
  writeln("lst ", lst);

  lst.set(2..4, ("oh", "no", 3)); // send a tuple
  writeln("lst ", lst);

  var chapelList = new List.list(int);
  chapelList.pushBack(11);
  lst.set(5..5, chapelList); // send a Chapel list
  writeln("lst ", lst);

  var arr = [true, true, false];
  lst.set(0..2, arr); // send an array
  writeln("lst ", lst);

  lst.remove(5);
  writeln("lst ", lst);

  lst.remove(1..3);
  writeln("lst ", lst);

  lst.set(0..1, arr); // check specifying less elements than contained in arr
  writeln("lst ", lst);

  lst.set(2.., ("blah", -2));
  writeln("lst ", lst);
  lst.set(..1, (11, false));
  writeln("lst ", lst);
  lst.set(.., ('a', 'b', 'c', 'd'));
  writeln("lst ", lst);

  lst.remove(3..);
  writeln("lst ", lst);
  lst.remove(..1);
  writeln("lst ", lst);
  lst.remove(..);
  writeln("lst ", lst);
}
