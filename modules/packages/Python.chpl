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
  private import Map;

  require "Python.h";
  private use CPythonInterface;
  private use CWChar;
  private use OS.POSIX only getenv;

  // interface typeConverter {
  //   proc Self.type_(type t) type { return nothing; }
  //   proc Self.handlesType(type t)param : bool { return Self.type_(t) != nothing; }
  //   proc Self.toPython(value: T): c_ptr(void) throws;
  //   proc Self.fromPython(obj: c_ptr(void)): T throws;
  // }

  private inline proc checkPyStatus(in status: PyStatus) throws {
    if PyStatus_Exception(status) {
      var str = string.createCopyingBuffer(status.err_msg);
      throw new Exception(str);
    }
  }

  /*
    There should only be one interpreter per program, it should be owned
  */
  class Interpreter {
    @chpldoc.nodoc
    var converters: List.list(owned TypeConverter);

    @chpldoc.nodoc
    proc init() throws {
      init this;

      // preinit
      var preconfig: PyPreConfig;
      PyPreConfig_InitPythonConfig(c_ptrTo(preconfig));
      preconfig.utf8_mode = 1;
      checkPyStatus(Py_PreInitialize(c_ptrTo(preconfig)));

      // init
      var config_: PyConfig;
      var cfgPtr = c_ptrTo(config_);
      PyConfig_InitPythonConfig(cfgPtr);
      defer PyConfig_Clear(cfgPtr);

      // set program name
      checkPyStatus(
        PyConfig_SetString(
          cfgPtr, c_ptrTo(config_.program_name), "chapel".c_wstr()));

      // check VIRTUAL_ENV, if its set, make it the executable
      var venv = getenv("VIRTUAL_ENV".c_str());
      if venv != nil {
        // ideally this just sets config_.home
        // but by setting executable we can reuse the python logic to determine
        // the locale (in the string sense, not the chapel sense)
        const executable = string.createBorrowingBuffer(venv) + "/bin/python";
        checkPyStatus(
          PyConfig_SetString(
            cfgPtr, c_ptrTo(config_.executable), executable.c_wstr()));
      }

      // initialize
      checkPyStatus(Py_InitializeFromConfig(cfgPtr));

      // add the current directory to the python path
      var sys = PyImport_ImportModule("sys");
      this.checkException();
      var path = PyObject_GetAttrString(sys, "path");
      this.checkException();
      PyList_Insert(path, 0, Py_BuildValue("s", "."));
      this.checkException();
      Py_DECREF(path);
      Py_DECREF(sys);
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
      if isIntType(t) {
        var v = Py_BuildValue("i", val);
        this.checkException();
        return v;
      } else if isRealType(t) {
        var v = Py_BuildValue("d", val);
        this.checkException();
        return v;
      } else if isBoolType(t) {
        var v = PyBool_FromLong(val:int);
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
      } else if isArrayType(t) && val.rank == 1 && val.isDefaultRectangular(){
        return toList(val);
      } else if isArrayType(t) && val.isAssociative() {
        return toDict(val);
      } else if isSubtype(t, List.list) {
        return toList(val);
      } else if isSubtype(t, Function) {
        return val.fn!.get();
      } else if isSubtype(t, PyObject) {
        return val.get();
      } else if isSubtype(t, ClassObject) {
        return val.obj!.get();
      } else if t == None {
        return nil;
      } else {
        halt("Unsupported toPython type: '" + t:string + "'");
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
      if isIntType(t) {
        var v = PyLong_AsLong(obj);
        this.checkException();
        return v: t; // cast to the real type
      } else if isRealType(t) {
        var v = PyFloat_AsDouble(obj);
        this.checkException();
        return v: t; // cast to the real type
      } else if isBoolType(t) {
        return if obj == Py_True then true else false;
      } else if t == c_ptrConst(c_char) {
        var v = PyUnicode_AsUTF8(obj);
        this.checkException();
        return v;
      } else if t == string {
        var v = string.createCopyingBuffer(PyUnicode_AsUTF8(obj));
        this.checkException();
        return v;
      } else if isArrayType(t) {
        return fromList(t, obj);
      } else if isSubtype(t, List.list) {
        return fromList(t, obj);
      } else if isSubtype(t, Function) {
        return new Function("<unknown>", new PyObject(this, obj));
      } else if isSubtype(t, PyObject) || isSubtype(t, PyObject?) {
        return new PyObject(this, obj);
      } else if isSubtype(t, ClassObject) || isSubtype(t, ClassObject?) {
        return new ClassObject(new PyObject(this, obj));
      } else if t == None {
        return new None();
      } else {
        halt("Unsupported fromPython type: '" + t:string + "'");
      }
    }

    /*
      Converts an array to a python list
    */
    @chpldoc.nodoc
    proc toList(arr): c_ptr(void) throws
      where isArrayType(arr.type) && arr.rank == 1 && arr.isDefaultRectangular() {
      var pyList = PyList_New(arr.size);
      this.checkException();
      for i in 0..<arr.size {
        const idx = arr.domain.orderToIndex(i);
        PyList_SetItem(pyList, i, toPython(arr[idx]));
        this.checkException();
      }
      return pyList;
    }

    /*
      Convert a chapel list to a python list
    */
    @chpldoc.nodoc
    proc toList(l: List.list(?)): c_ptr(void) throws {
      var pyList = PyList_New(l.size);
      this.checkException();
      for i in 0..<l.size {
        PyList_SetItem(pyList, i, toPython(l(i)));
        this.checkException();
      }
      return pyList;
    }

    /*
      Converts a python list to an array
    */
    @chpldoc.nodoc
    proc fromList(type T, obj: c_ptr(void)): T throws
      where isArrayType(T) {
      var res: T;
      if PySequence_Size(obj) != res.size {
        throw new Exception("Size mismatch");
      }
      for i in 0..<res.size {
        const idx = res.domain.orderToIndex(i);
        res[idx] = fromPython(res.eltType, PySequence_GetItem(obj, i));
        this.checkException();
      }
      return res;
    }

    /*
      Convert a python list to a Chapel list
    */
    @chpldoc.nodoc
    proc fromList(type T, obj: c_ptr(void)): T throws where isSubtype(T, List.list) {
      var res = new T();
      while true {
        var item = PyIter_Next(obj);
        if item == nil {
          break;
        }
        res.pushBack(fromPython(T.eltType, item));
        this.checkException();
      }
      return res;
    }

    /*
      Converts an array to a python set
    */
    // proc toSet(arr): c_ptr(void) throws {
    // TODO
    // }

    // TODO: convert chapel set to python set

    /*
      Converts a python set to an array
    */
    // proc fromSet(type T, obj: c_ptr(void)): T throws {
    // TODO
    // }

    // TODO: convert python set to chapel set


    /*
      Converts an associative array to a python dictionary
    */
    @chpldoc.nodoc
    proc toDict(arr): c_ptr(void) throws
      where isArrayType(arr.type) && arr.isAssociative() {
      var pyDict = PyDict_New();
      this.checkException();
      for key in arr.domain {
        var pyKey = toPython(key);
        var pyValue = toPython(arr[key]);
        PyDict_SetItem(pyDict, pyKey, pyValue);
        this.checkException();
      }
      return pyDict;
    }

    // TODO: convert chapel map to python dict

    /*
      Converts a python dictionary to an associative array
    */
    // proc fromDict(type T, obj: c_ptr(void)): T throws {
      // TODO
    // }

    // TODO: convert python dict to chapel map

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


  // TODO: make a base Value class that can be used for all values
  //  the base class is essentially what PyObject is now

  // TODO: manufacture a custom ArrayAdapter for Chapel arrays, allows for easy conversion to python types without copying
  //  this can be done in a few ways.
  //   1. create a class that wraps the array and implements the python list interface
  //   2. create a class that wraps the array and implements the numpy array interface

  //  TODO: represent python context managers as Chapel context managers

  // TODO: create adapters for common Python types that are subclasses of a Value class
  // this will prevent needing to round trip through python for some operations
  // for example, a List, Set, Dict, Tuple, etc.
  // these should provide native like operation, so `for i in pyList` should work

  // TODO: there are decrefs missing all over the place

  // TODO: using the Py*_Check, we may be able to avoid needing to specify the type of the return value

  // TODO: implement operators as dunder methods

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

      var pyRes;
      if pyArgs.size == 1 then
        pyRes = PyObject_CallOneArg(this.fn!.get(), pyArgs(0));
      else
        pyRes = PyObject_CallFunctionObjArgs(this.fn!.get(), (...pyArgs), nil);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);

      return res;
    }
    proc this(type retType): retType throws {
      var pyRes = PyObject_CallNoArgs(this.fn!.get());
      interpreter.checkException();
      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }
    proc this(type retType, args..., kwargs:?t=none): retType throws
      where kwargs.domain.isAssociative() {
      var pyArgs: args.size * c_ptr(void);
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }
      var pyArg = PyTuple_Pack(args.size, (...pyArgs));

      var pyKwargs;
      if t != nothing {
        pyKwargs = interpreter.toPython(kwargs);
      } else {
        pyKwargs = nil;
      }

      var pyRes = PyObject_Call(this.fn!.get(), pyArg, pyKwargs);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);

      return res;
    }

    proc getAttr(type t, attr: string): t throws {
      var pyAttr = PyObject_GetAttrString(this.fn!.get(), attr.c_str());
      interpreter.checkException();
      var res = interpreter.fromPython(t, pyAttr);
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

      var pyRes = PyObject_CallFunctionObjArgs(this.cls.get(), (...pyArgs), nil);
      interpreter.checkException();

      return new PyObject(interpreter, pyRes);
    }
    @chpldoc.nodoc
    proc newInstance(): owned PyObject throws {
      var pyRes = PyObject_CallNoArgs(this.cls.get());
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
      return res;
    }
    proc setAttr(attr: string, value) throws {
      var pyValue = interpreter.toPython(value);
      PyObject_SetAttrString(this.obj!.get(), attr.c_str(), pyValue);
      interpreter.checkException();
    }

    proc this(type retType, args...): retType throws {
      var pyArgs: args.size * c_ptr(void);
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }
      var pyRes;
      if pyArgs.size == 1 then
        pyRes = PyObject_CallOneArg(this.obj!.get(), pyArgs(0));
      else
        pyRes = PyObject_CallFunctionObjArgs(this.obj!.get(), (...pyArgs), nil);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);

      return res;
    }

    proc call(type retType, method: string, args...): retType throws {
      var pyArgs: args.size * c_ptr(void);
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }

      var methodName = interpreter.toPython(method);
      var pyRes = PyObject_CallMethodObjArgs(
        this.obj!.get(), methodName, (...pyArgs), nil);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);

      return res;
    }
    proc call(type retType, method: string): retType throws {
      var methodName = interpreter.toPython(method);
      var pyRes = PyObject_CallMethodNoArgs(this.obj!.get(), methodName);
      interpreter.checkException();
      var res = interpreter.fromPython(retType, pyRes);
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
    @chpldoc.nodoc
    proc incrementRC() {
      Py_INCREF(this.obj);
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
  module CWChar {
    require "wchar.h";
    private use CTypes;

    extern "wchar_t" type c_wchar;
    extern proc mbstowcs(dest: c_ptr(c_wchar), src: c_ptrConst(c_char), n: c_size_t): c_size_t;

    proc string.c_wstr(): c_ptr(c_wchar) {
      var len = this.size;
      var buf = allocate(c_wchar, len + 1, clear=true);
      mbstowcs(buf, this.c_str(), len);
      return buf;
    }
  }

  @chpldoc.nodoc
  module CPythonInterface {
    private use CTypes;
    private use super.CWChar;

    /*
      PyConfig
    */
    extern record PyConfig {
      // TODO: this fails LLVM ir verification, because the fields are out of order
      // but the chapel spec says the order doesn't matter?
      var isolated: c_int;
      var user_site_directory: c_int;
      var site_import: c_int;
      var module_search_paths: PyWideStringList;
      var module_search_paths_set: c_int;
      var home: c_ptr(c_wchar);
      var use_environment: c_int;
      var filesystem_encoding: c_ptr(c_wchar);
      var program_name: c_ptr(c_wchar);
      var executable: c_ptr(c_wchar);
    }
    extern proc PyConfig_Clear(config_: c_ptr(PyConfig));
    extern proc PyConfig_Read(config_: c_ptr(PyConfig)): PyStatus;
    extern proc PyConfig_InitIsolatedConfig(config_: c_ptr(PyConfig));
    extern proc PyConfig_InitPythonConfig(config_: c_ptr(PyConfig));

    extern proc PyConfig_SetString(config_: c_ptr(PyConfig), config_str: c_ptr(c_ptr(c_wchar)), str: c_ptr(c_wchar)): PyStatus;
    extern proc PyConfig_SetBytesString(config_: c_ptr(PyConfig), config_str: c_ptr(c_ptr(c_wchar)), str: c_ptrConst(c_char)): PyStatus;

    extern type PyWideStringList;
    extern proc PyWideStringList_Append(list: c_ptr(PyWideStringList), str: c_ptr(c_wchar)): PyStatus;
    extern proc PyWideStringList_Insert(list: c_ptr(PyWideStringList), idx: c_int, str: c_ptr(c_wchar)): PyStatus;

    /*
      PyPreConfig
    */
    extern record PyPreConfig {
      var isolated: c_int;
      var utf8_mode: c_int;
    }
    extern proc PyPreConfig_InitPythonConfig(config_: c_ptr(PyPreConfig));
    extern proc Py_PreInitialize(config_: c_ptr(PyPreConfig)): PyStatus;

    /*
      Global functions for the interpreter
    */
    extern record PyStatus {
      var err_msg: c_ptrConst(c_char);
    }
    extern proc Py_Initialize();
    extern proc Py_InitializeFromConfig(config_: c_ptr(PyConfig)): PyStatus;
    extern proc PyStatus_Exception(in status: PyStatus): bool;
    extern proc Py_Finalize();
    extern proc Py_INCREF(obj: c_ptr(void));
    extern proc Py_DECREF(obj: c_ptr(void));
    extern proc PyObject_Str(obj: c_ptr(void)): c_ptr(void); // `str(obj)`
    extern proc PyImport_ImportModule(name: c_ptrConst(c_char)): c_ptr(void);


    /*
      Global exec functions
    */
    extern proc PyRun_SimpleString(code: c_ptrConst(c_char));

    //python 3.13 only
    // extern proc PyEval_GetFrameGlobals(): c_ptr(void);
    extern "PyEval_GetGlobals" proc PyEval_GetFrameGlobals(): c_ptr(void);


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
    extern proc PyLong_FromLong(v: c_long): c_ptr(void);
    extern proc PyLong_AsLong(obj: c_ptr(void)): c_long;
    extern proc PyLong_AsInt(obj: c_ptr(void)): c_int;
    extern proc PyLong_FromUnsignedLong(v: c_ulong): c_ptr(void);
    extern proc PyLong_AsUnsignedLong(obj: c_ptr(void)): c_ulong;
    extern proc PyFloat_FromDouble(r: real(64)): c_ptr(void);
    extern proc PyFloat_AsDouble(obj: c_ptr(void)): real(64);
    extern proc PyBool_FromLong(v: c_long): c_ptr(void);
    extern proc PyString_FromString(s: c_ptrConst(c_char)): c_ptr(void);
    extern proc PyUnicode_AsUTF8(obj: c_ptr(void)): c_ptr(c_char);

    extern var Py_False: c_ptr(void);
    extern var Py_True: c_ptr(void);

    /*
      Sequences
    */
    extern proc PySequence_Size(obj: c_ptr(void)): c_long;
    extern proc PySequence_GetItem(obj: c_ptr(void), idx: c_long): c_ptr(void);
    extern proc PySequence_SetItem(obj: c_ptr(void), idx: c_long, value: c_ptr(void));


    /*
      Iterators
    */
    extern proc PyIter_Next(obj: c_ptr(void)): c_ptr(void);
    extern proc PyIter_Check(obj: c_ptr(void)): c_int;

    /*
      Lists
    */
    extern proc PyList_New(size: c_long): c_ptr(void);
    extern proc PyList_SetItem(list: c_ptr(void), idx: c_long, item: c_ptr(void));
    extern proc PyList_GetItem(list: c_ptr(void), idx: c_long): c_ptr(void);
    extern proc PyList_Size(list: c_ptr(void)): c_long;
    extern proc PyList_Insert(list: c_ptr(void), idx: c_long, item: c_ptr(void));

    /*
      Sets
    */
    extern proc PySet_New(): c_ptr(void);
    extern proc PySet_Add(set: c_ptr(void), item: c_ptr(void));
    extern proc PySet_Size(set: c_ptr(void)): c_long;


    /*
      Dictionaries
    */
    extern proc PyDict_New(): c_ptr(void);
    extern proc PyDict_SetItem(dict: c_ptr(void), key: c_ptr(void), value: c_ptr(void));
    extern proc PyDict_SetItemString(dict: c_ptr(void), key: c_ptrConst(c_char), value: c_ptr(void));
    extern proc PyDict_GetItem(dict: c_ptr(void), key: c_ptr(void)): c_ptr(void);
    extern proc PyDict_GetItemString(dict: c_ptr(void), key: c_ptrConst(c_char)): c_ptr(void);
    extern proc PyDict_Size(dict: c_ptr(void)): c_long;

    extern proc PyObject_GetAttrString(obj: c_ptr(void), name: c_ptrConst(c_char)): c_ptr(void);
    extern proc PyObject_SetAttrString(obj: c_ptr(void), name: c_ptrConst(c_char), value: c_ptr(void));

    /*
      Tuples
    */
    extern proc PyTuple_Pack(size: c_long, args...): c_ptr(void);

    /*
      Functions
    */
    extern proc PyObject_Call(obj: c_ptr(void), args: c_ptr(void), kwargs: c_ptr(void)): c_ptr(void);
    extern proc PyObject_CallObject(obj: c_ptr(void), args: c_ptr(void)): c_ptr(void);
    extern proc PyObject_CallNoArgs(obj: c_ptr(void)): c_ptr(void);
    extern proc PyObject_CallOneArg(obj: c_ptr(void), arg: c_ptr(void)): c_ptr(void);
    extern proc PyObject_CallFunctionObjArgs(obj: c_ptr(void), args...): c_ptr(void);

    extern proc PyObject_CallMethodNoArgs(obj: c_ptr(void), method: c_ptr(void)): c_ptr(void);
    extern proc PyObject_CallMethodObjArgs(obj: c_ptr(void), method: c_ptr(void), args...): c_ptr(void);



  }


}
