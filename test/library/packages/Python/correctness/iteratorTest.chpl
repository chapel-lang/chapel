use Python;
import Reflection;
import List;

config const print = false;
config const low = 2;
config const high = 10;

proc test(func: borrowed Function) {
  var check = [i in low..<high] i;

  writeln("Testing ", func.fnName);

  {
    // directly convert to a list
    var res = func(List.list(int), low, high);
    if print then writeln("  list: ", res);

    // the domains may not be the same between check and `.toArray`
    for (expected, actual) in zip(check, res) {
      assert(expected == actual);
    }
  }

  {
    // convert to a list via a PyObject
    var obj = func(owned PyObject, low, high);
    if print then writeln("  PyObject: ", obj.str());

    var res = obj.value(List.list(int));
    if print then writeln("  from PyObject to list: ", res);

    // the domains may not be the same between check and `.toArray`
    for (expected, actual) in zip(check, res) {
      assert(expected == actual);
    }
  }


}

proc main() {
  var interp = new Interpreter();

  var modName = Reflection.getModuleName();
  var m = new Module(interp, modName);

  var myiter = new Function(m, "myiter");
  test(myiter);

  var mygen = new Function(m, "mygen");
  test(mygen);

}
