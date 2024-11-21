use Python;
import Reflection;
use CTypes;

record myRec {
  var x: int;
  var y: string;
}

class myRecConverter: TypeConverter {
  var pyClsType: borrowed Class;
  override proc handlesType(type T): bool {
    return T == myRec;
  }
  override proc toPython(interpreter: borrowed Interpreter,
                         type T, const value: T): Python.CPythonInterface.PyObjectPtr throws {
    if T != myRec then halt("Expected myRec");
    return PyObject.release(pyClsType(value.x, value.y));
  }
  override proc fromPython(interpreter: borrowed Interpreter,
                           type T, obj: Python.CPythonInterface.PyObjectPtr): T throws {
    if T != myRec then halt("Expected myRec");
    var cls = new ClassObject(new PyObject(interpreter, obj));
    var res = new myRec(cls.getAttr(int, "x"), cls.getAttr(string, "y"));
    return res;
  }
}

proc main() {
  var interp = new Interpreter();

  var modName = Reflection.getModuleName();
  var m = new Module(interp, modName);

  var pyClsType = new Class(m, "MyRec");
  interp.registerConverter(new myRecConverter(pyClsType));

  var printAndReturn = new Function(m, "printAndReturn");
  var fromPy = printAndReturn(myRec, new myRec(42, "hello"));
  writeln(fromPy);
}
