/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* A library for interfacing with Python from Chapel code.

  TODO: docs about this library

  To build Chapel code that uses this module, you will need to include the Python
  header files and link against the Python library. This can be done by setting
  the ``--ccflags`` and ``--ldflags`` options when compiling Chapel code.
  The following example shows how to build Chapel code that uses this module:

  .. code-block:: bash

     PYTHON_INCLUDE_DIR=$(python3 -c "import sysconfig; print(sysconfig.get_paths()['include'])")
     PYTHON_LIB_DIR=$(python3 -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))")
     PYTHON_LDVERSION=$(python3 -c "import sysconfig; print(sysconfig.get_config_var('LDVERSION'))")

     chpl --ccflags -isystem$PYTHON_INCLUDE_DIR \
          -L$PYTHON_LIB_DIR -lpython$PYTHON_LDVERSION ...Chapel source files...


  TODO: example codes
*/
@unstable("Python is unstable")
module Python {
  private use CTypes;
  private import List;

  require "Python.h";
  private use CPythonInterface;

  // interface typeConverter {
  //   proc Self.type_(type t) type { return nothing; }
  //   proc Self.handlesType(type t)param : bool { return Self.type_(t) != nothing; }
  //   proc Self.toPython(value: T): c_ptr(void) throws;
  //   proc Self.fromPython(obj: c_ptr(void)): T throws;
  // }


  /*
    There should only be one interpreter per program, it should be owned
  */
  class Interpreter {
    @chpldoc.nodoc
    var converters: List.list(owned TypeConverter);

    @chpldoc.nodoc
    proc init() {
      init this;
      Py_Initialize();
    }
    @chpldoc.nodoc
    proc deinit() {
      Py_Finalize();
    }
    /*
      Run a string of python code
    */
    proc run(code: string) throws {
      PyRun_SimpleString(code.c_str());
      this.checkException();
    }

    /*
      Register a custom type converter
    */
    proc registerConverter(in cvt: owned TypeConverter) {
      converters.pushBack(cvt);
    }

    /*
      Compile a lambda function like 'lambda x,: x + 1'

      Note: this only works with lambdas that a tuple of arguments, like 'x,' or '*args'
    */
    @chpldoc.nodoc
    proc compileLambda(l: string): owned PyObject throws {
      var globals = PyEval_GetFrameGlobals();
      // create a globals if it doesn't exist
      if !globals {
        globals = Py_BuildValue("{}"); // TODO: I need to decrement this?
      }
      // this is the equivalent of `eval(compile(l, '<string>', 'eval'))`
      // we can also do `Py_CompileString` -> `PyFunction_New`?
      var code = PyRun_String(l.c_str(), Py_eval_input, globals, nil);
      this.checkException();
      var obj = new PyObject(this, code);
      return obj;
    }

    /*
      Query to see if an exception has occurred, it will print the exception and halt
    */
    inline proc checkException() throws {
      var ptype, pvalue, ptraceback: c_ptr(void);
      PyErr_Fetch(c_ptrTo(ptype), c_ptrTo(pvalue), c_ptrTo(ptraceback));

      if pvalue {
        var pvalue_str = PyObject_Str(pvalue);
        var value_str = this.fromPython(string, pvalue_str);
        Py_DECREF(pvalue_str);
        throw new Exception(value_str);
      }
    }

    /*
      Convert a Chapel value to a python object

      Note: unless you are writing a custom type converter, you probably don't need to use this
    */
    proc toPython(val: ?t): c_ptr(void) throws {
      for converter in this.converters {
        if converter.handlesType(t) {
          return converter.toPython(this, t, val);
        }
      }
      // if no converter is found, use the defaults
      if t == int {
        var v = Py_BuildValue("i", val);
        this.checkException();
        return v;
      } else if t == c_ptrConst(c_char) {
        var v = Py_BuildValue("s", val);
        this.checkException();
        return v;
      } else if t == string {
        var v = Py_BuildValue("s", val.c_str());
        this.checkException();
        return v;
      } else if isArrayType(t) && val.rank == 1{
        var pyList = PyList_New(val.size);
        this.checkException();
        for i in 0..<val.size {
          const idx = val.domain.orderToIndex(i);
          PyList_SetItem(pyList, i, toPython(val[idx]));
          this.checkException();
        }
        return pyList;
      } else if isSubtype(t, Function) {
        return val.fn!.get();
      } else if t == PyObject {
        return val.get();
      } else if t == None {
        return nil;
      } else {
        halt("Unsupported type");
      }
    }

    /*
      Convert a python object to a Chapel value

      Note: unless you are writing a custom type converter, you probably don't need to use this
    */
    proc fromPython(type t, obj: c_ptr(void)): t throws {
      for converter in this.converters {
        if converter.handlesType(t) {
          return converter.fromPython(this, t, obj);
        }
      }
      // if no converter is found, use the defaults
      if t == int {
        var v = PyLong_AsLong(obj);
        this.checkException();
        return v;
      } else if t == c_ptrConst(c_char) {
        var v = PyUnicode_AsUTF8(obj);
        this.checkException();
        return v;
      } else if t == string {
        var v = string.createCopyingBuffer(PyUnicode_AsUTF8(obj));
        this.checkException();
        return v;
      } else if isArrayType(t) {
        var res: t;
        if PyList_Size(obj) != res.size {
          throw new Exception("Size mismatch");
        }
        for i in 0..<res.size {
          const idx = res.domain.orderToIndex(i);
          res[idx] = fromPython(res.eltType, PyList_GetItem(obj, i));
          this.checkException();
        }
        return res;
      } else if isSubtype(t, List.list) {
        var res = new t();
        for i in 0..<PyList_Size(obj) {
          res.pushBack(fromPython(t.eltType, PyList_GetItem(obj, i)));
          this.checkException();
        }
        return res;
      } else if isSubtype(t, Function) {
        return new Function("<unknown>", new PyObject(this, obj));
      } else if isSubtype(t, PyObject) || isSubtype(t, PyObject?) {
        return new PyObject(this, obj);
      } else if isSubtype(t, ClassObject) || isSubtype(t, ClassObject?) {
        return new ClassObject(new PyObject(this, obj));
      } else if t == None {
        return new None();
      } else {
        halt("Unsupported type " + t:string);
      }
    }

  }

  /*
    Represents a python error
  */
  class Exception: Error {
    proc init(in message: string) {
      super.init(message);
    }
  }


  //
  // TODO: using an interface is the ideal here, but there are too many limitations
  //
  // interface typeConverter {
  //   proc Self.handlesType(type T): bool;
  //   proc Self.toPython(interpreter: borrowed Interpreter,
  //                      type T,
  //                      ref value: T): c_ptr(void) throws;
  //   proc Self.fromPython(interpreter: borrowed Interpreter,
  //                        type T,
  //                        obj: c_ptr(void)): T throws;
  // }

  class TypeConverter {
    proc handlesType(type T): bool {
      return false;
    }
    proc toPython(interpreter: borrowed Interpreter, type T, value: T): c_ptr(void) throws {
      import HaltWrappers;
      HaltWrappers.pureVirtualMethodHalt();
    }
    proc fromPython(interpreter: borrowed Interpreter, type T, obj: c_ptr(void)): T throws {
      import HaltWrappers;
      HaltWrappers.pureVirtualMethodHalt();
    }
  }


  /*
    Represents the Global Interpreter Lock, this is used to ensure that only one thread is executing python code at a time
  */
  record GIL {
    var state: PyGILState_STATE;
    proc init() {
      state = PyGILState_Ensure();
    }
    proc deinit() {
      PyGILState_Release(state);
    }
  }



  /*
    Represents a python module
  */
  class Module {
    var modName: string;
    var mod: owned PyObject;
    proc init(interpreter: borrowed Interpreter, in modName: string) throws {
      this.modName = modName;
      this.mod = new PyObject(interpreter, PyImport_ImportModule(modName.c_str()));
      init this;
      this.mod.check();
    }
    proc interpreter do return this.mod.interpreter;
    proc str(): string throws {
      return this.mod!.str();
    }
  }

  /*
    Represents a python function
  */
  class Function {
    var fnName: string;
    var fn: owned PyObject?;
    proc init(mod: borrowed Module, in fnName: string) throws {
      this.fnName = fnName;
      this.fn = new PyObject(mod.mod.interpreter, PyObject_GetAttrString(mod.mod.get(), fnName.c_str()));
      init this;
      this.fn!.check();
    }
    proc init(in fnName: string, in fn: owned PyObject) throws {
      this.fnName = fnName;
      this.fn = fn;
      init this;
      this.fn!.check();
    }
    proc init(interpreter: borrowed Interpreter, in lambdaFn: string) throws {
      this.fnName = "<anon>";
      init this;
      this.fn = interpreter.compileLambda(lambdaFn);
      this.fn!.check();
    }
    proc interpreter do return this.fn!.interpreter;
    proc str(): string throws {
      return this.fn!.str();
    }

    /*
      Call a python function with Chapel arguments and get a Chapel return value
    */
    proc this(type retType, args...): retType throws {
      var pyArgs: args.size * c_ptr(void);
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }
      var pyArg = PyTuple_Pack(args.size, (...pyArgs));
      interpreter.checkException();

      var pyRes = PyObject_CallObject(this.fn!.get(), pyArg);
      interpreter.checkException();

      Py_DECREF(pyArg);

      var res = interpreter.fromPython(retType, pyRes);
      Py_DECREF(pyRes);

      return res;
    }
    proc this(type retType): retType throws {
      var pyRes = PyObject_CallObject(this.fn!.get(), nil);
      interpreter.checkException();
      var res = interpreter.fromPython(retType, pyRes);
      Py_DECREF(pyRes);
      return res;
    }
  }


  /*
    Represents a python class type
  */
  class Class {
    var className: string;
    var cls: owned PyObject;
    proc init(mod: borrowed Module, in className: string) throws {
      this.className = className;
      this.cls = new PyObject(mod.mod.interpreter, PyObject_GetAttrString(mod.mod.get(), className.c_str()));
      init this;
      this.cls.check();
    }
    proc interpreter do return this.cls.interpreter;
    proc str(): string throws {
      return this.cls!.str();
    }

    /*
      Create a new instance of a python class
    */
    proc newInstance(args...): owned PyObject throws {
      var pyArgs: args.size * c_ptr(void);
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }
      var pyArg = PyTuple_Pack(args.size, (...pyArgs));
      interpreter.checkException();

      var pyRes = PyObject_CallObject(this.cls.get(), pyArg);
      interpreter.checkException();

      Py_DECREF(pyArg);

      return new PyObject(interpreter, pyRes);
    }
    @chpldoc.nodoc
    proc newInstance(): owned PyObject throws {
      var pyRes = PyObject_CallObject(this.cls.get(), nil);
      interpreter.checkException();
      return new PyObject(interpreter, pyRes);
    }

    /*
      Create a new instance of a python class
    */
    proc this(args...): owned ClassObject throws {
      return new ClassObject(this, (...args));
    }
    proc this(): owned ClassObject throws {
      return new ClassObject(this);
    }


  }


  /*
    Represents a python value
  */
  class ClassObject {
    var cls: borrowed Class?;
    var obj: owned PyObject?;
    proc init(cls: borrowed Class, args...) throws {
      this.cls = cls;
      init this;
      this.obj = cls.newInstance((...args));
      this.obj!.check();
    }
    proc init(cls: borrowed Class) throws {
      this.cls = cls;
      init this;
      this.obj = cls.newInstance();
      this.obj!.check();
    }
    proc init(in obj: owned PyObject?) throws {
      this.obj = obj;
      init this;
      this.obj!.check();
    }
    proc interpreter do return this.obj!.interpreter;
    proc str(): string throws {
      return this.obj!.str();
    }

    proc getAttr(type t, attr: string): t throws {
      var pyAttr = PyObject_GetAttrString(this.obj!.get(), attr.c_str());
      interpreter.checkException();
      var res = interpreter.fromPython(t, pyAttr);
      Py_DECREF(pyAttr);
      return res;
    }
    proc setAttr(attr: string, value) throws {
      var pyValue = interpreter.toPython(value);
      PyObject_SetAttrString(this.obj!.get(), attr.c_str(), pyValue);
      interpreter.checkException();
    }
    proc callMethod(type retType, method: string, args...): retType throws {
      var pyArgs: args.size * c_ptr(void);
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }
      var pyArg = PyTuple_Pack(args.size, (...pyArgs));
      interpreter.checkException();

      var pyRes = PyObject_CallObject(PyObject_GetAttrString(this.obj!.get(), method.c_str()), pyArg);
      interpreter.checkException();

      Py_DECREF(pyArg);

      var res = interpreter.fromPython(retType, pyRes);
      Py_DECREF(pyRes);

      return res;
    }
    proc callMethod(type retType, method: string): retType throws {
      var pyRes = PyObject_CallObject(PyObject_GetAttrString(this.obj!.get(), method.c_str()), nil);
      interpreter.checkException();
      var res = interpreter.fromPython(retType, pyRes);
      Py_DECREF(pyRes);
      return res;
    }
  }


  /*
    Represents a python object, it handles reference counting and is owned by default.

    Most users should not need to use this directly.
  */
  class PyObject {
    var interpreter: borrowed Interpreter;
    var obj: c_ptr(void);
    var isOwned: bool;

    proc init(in interpreter: borrowed Interpreter, obj: c_ptr(void), isOwned: bool = true) {
      this.interpreter = interpreter;
      this.obj = obj;
      this.isOwned = isOwned;
    }
    proc init(in interpreter: borrowed Interpreter, value: ?) {
      this.interpreter = interpreter;
      this.obj = toPython(interpreter, value);
      this.isOwned = true;
    }
    proc deinit() {
      if this.isOwned then
        Py_DECREF(this.obj);
    }
    proc check() throws {
      // if obj == nil {
      //   throw new Exception("Failed to get object");
      // }
      this.interpreter.checkException();
    }
    proc get() {
      return this.obj;
    }
    proc value(type value) {
      return interpreter.fromPython(value, this.obj);
    }
    proc str(): string throws {
      var pyStr = PyObject_Str(this.obj);
      interpreter.checkException();
      var res = interpreter.fromPython(string, pyStr);
      Py_DECREF(pyStr);
      return res;
    }
  }

  /* Represents the python value 'None' */
  record None {
    proc str(): string {
      return "None";
    }
  }

  operator:(v, type t: string): string throws
    where isSubtype(v.type, PyObject) || isSubtype(v.type, Module) ||
          isSubtype(v.type, Class) || isSubtype(v.type, Function) ||
          isSubtype(v.type, ClassObject) || isSubtype(v.type, None) {
    return v.str();
  }
  // PyObject intentionally does not have a serialize method
  // its meant to be an implementation detail and not used directly
  Module implements writeSerializable;
  override proc Module.serialize(writer, ref serializer) throws do
    writer.write(this:string);
  Function implements writeSerializable;
  override proc Function.serialize(writer, ref serializer) throws do
    writer.write(this:string);
  Class implements writeSerializable;
  override proc Class.serialize(writer, ref serializer) throws do
    writer.write(this:string);
  ClassObject implements writeSerializable;
  override proc ClassObject.serialize(writer, ref serializer) throws do
    writer.write(this:string);
  None implements writeSerializable;
  proc None.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  module CPythonInterface {
    private use CTypes;

    /*
      Global functions for the interpreter
    */
    extern proc Py_Initialize();
    extern proc Py_Finalize();
    extern proc Py_INCREF(obj: c_ptr(void));
    extern proc Py_DECREF(obj: c_ptr(void));
    extern proc PyObject_Str(obj: c_ptr(void)): c_ptr(void); // `str(obj)`
    extern proc PyImport_ImportModule(name: c_ptrConst(c_char)): c_ptr(void);

    /*
      Global exec functions
    */
    extern proc PyRun_SimpleString(code: c_ptrConst(c_char));
    extern proc PyEval_GetFrameGlobals(): c_ptr(void);
    extern var Py_eval_input: c_int;
    extern proc PyRun_String(code: c_ptrConst(c_char), start: c_int, globals: c_ptr(void), locals: c_ptr(void)): c_ptr(void);

    /*
      Threading
    */
    extern proc PyEval_SaveThread(): c_ptr(void);
    extern proc PyEval_RestoreThread(state: c_ptr(void));

    extern type PyGILState_STATE;
    extern proc PyGILState_Ensure(): PyGILState_STATE;
    extern proc PyGILState_Release(state: PyGILState_STATE);

    /*
      Error handling
    */
    extern proc PyErr_Occurred(): c_ptr(void);
    extern proc PyErr_Fetch(ptype: c_ptr(c_ptr(void)),
                            pvalue: c_ptr(c_ptr(void)),
                            ptraceback: c_ptr(c_ptr(void)));

    /*
      Values
    */
    extern proc Py_BuildValue(format: c_ptrConst(c_char), vals...): c_ptr(void);
    extern proc Py_BuildValue(format: c_ptrConst(c_char)): c_ptr(void);
    extern proc PyLong_AsLong(obj: c_ptr(void)): c_long;
    extern proc PyString_FromString(s: c_ptrConst(c_char)): c_ptr(void);
    extern proc PyUnicode_AsUTF8(obj: c_ptr(void)): c_ptr(c_char);

    /*
      Lists
    */
    extern proc PyList_New(size: c_long): c_ptr(void);
    extern proc PyList_SetItem(list: c_ptr(void), idx: c_long, item: c_ptr(void));
    extern proc PyList_GetItem(list: c_ptr(void), idx: c_long): c_ptr(void);
    extern proc PyList_Size(list: c_ptr(void)): c_long;

    /*
      Dictionaries
    */
    extern proc PyObject_GetAttrString(obj: c_ptr(void), name: c_ptrConst(c_char)): c_ptr(void);
    extern proc PyObject_SetAttrString(obj: c_ptr(void), name: c_ptrConst(c_char), value: c_ptr(void));

    /*
      Tuples
    */
    extern proc PyTuple_Pack(size: c_long, args...): c_ptr(void);

    /*
      Functions
    */
    extern proc PyObject_CallObject(obj: c_ptr(void), args: c_ptr(void)): c_ptr(void);



  }


}
