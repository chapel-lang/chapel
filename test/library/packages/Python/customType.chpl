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
  override proc toPython(interpreter: borrowed Interpreter, type T, value: T): c_ptr(void) throws {
    if T != myRec then halt("Expected myRec");
    var pyObj: unmanaged PyObject = owned.release(pyClsType.newInstance(value.x, value.y));
    pyObj.isOwned = false;
    var ptr = pyObj.get();
    delete pyObj;
    return ptr;
  }
  override proc fromPython(interpreter: borrowed Interpreter, type T, obj: c_ptr(void)): T throws {
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
