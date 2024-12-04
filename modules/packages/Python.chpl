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

// TODO: using the Py*_Check, we may be able to avoid needing to specify the type of the return value

// TODO: implement operators as dunder methods

// TODO: add the ability to compile a chapel string into a python module/function/class/whatever

// TODO: make python use chapel stdout/stderr

/* Library for interfacing with Python from Chapel code.

  This module provides a Chapel interface to a Python interpreter.
  It allows manipulating native Python objects and libraries with minimal
  wrapper code required.

  .. note::

     This module is implemented using the Python C API, and as such, it is not
     compatible with PyPy or other alternative Python implementations.

  Compiling Chapel code
  ---------------------

  Compiling Chapel code that uses this module needs the ``Python.h`` header file
  and requires linking with the Python library. If ``python3`` is installed,
  this can be achieved with the following commands:

  .. code-block:: bash

     PYTHON_INCLUDE_DIR=$(python3 -c "import sysconfig; print(sysconfig.get_paths()['include'])")
     PYTHON_LIB_DIR=$(python3 -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))")
     PYTHON_LDVERSION=$(python3 -c "import sysconfig; print(sysconfig.get_config_var('LDVERSION'))")

     chpl --ccflags -isystem$PYTHON_INCLUDE_DIR \
          -L$PYTHON_LIB_DIR --ldflags -Wl,-rpath,$PYTHON_LIB_DIR \
          -lpython$PYTHON_LDVERSION ...Chapel source files...

  Parallel Execution
  ------------------

  Running any Python code in parallel from Chapel requires special care. Before
  any parallel execution with Python code can occur, the thread state needs to
  be saved. After the parallel execution, the thread state must to be restored.
  Then for each thread, the Global Interpreter Lock (GIL) must be acquired and
  released. This is necessary to prevent segmentation faults and deadlocks in
  the Python interpreter.

  The following demonstrates this when explicit tasks are introduced with a
  ``coforall``:

  ..
     START_TEST
     FILENAME: CoforallTest.chpl
     START_GOOD
     1 4 9 16 25 36 49 64 81 100
     END_GOOD

  .. code-block:: chapel

     use Python;

     var Arr: [1..10] int = 1..10;

     var interp = new Interpreter();
     var func = new Function(interp, "lambda x,: x * x");

     var ts = new threadState();
     ts.save(); // save the thread state
     coforall tid in 1..10 {
       var gil = new GIL(); // acquire the GIL
       Arr[tid] = func(int, tid);
       // GIL is automatically released at the end of the block
     }
     ts.restore(); // restore the thread state
     writeln(Arr);

  ..
     END_TEST

  When using a data-parallel ``forall``, the GIL should be acquired and released
  as a task private variable.

  ..
     START_TEST
     FILENAME: ForallTest.chpl
     START_GOOD
     1 4 9 16 25 36 49 64 81 100
     END_GOOD

  .. code-block:: chapel

     use Python;

     var Arr: [1..10] int = 1..10;

     var interp = new Interpreter();
     var func = new Function(interp, "lambda x,: x * x");

     var ts = new threadState();
     ts.save(); // save the thread state
     forall tid in 1..10 with (var gil = new GIL()) {
       Arr[tid] = func(int, tid);
     }
     ts.restore(); // restore the thread state
     writeln(Arr);

  ..
     END_TEST

  In the examples above, because the GIL is being acquired and released for the
  entirety of each task, these examples will be no faster than running the tasks
  serially.

  .. note::

     Newer Python versions offer a free-threading mode that allows multiple threads concurrently, without the need for the GIL. In this mode, users can either remove the GIL acquisition code or not. Without the GIL, the GIL acquisition code will have no effect.

  .. note::

     In the future, it may be possible to achieve better parallelism with Python
     by using sub-interpreters. However, sub-interpreters are not yet supported
     in Chapel.

  Using Python Modules With Distributed Code
  -------------------------------------------

  Python has no built-in support for distributed memory, so each locale must create its own interpreter (and subsequent Python objects).

  The following example demonstrates how to create a Python interpreter and run a Python function on each locale:

  ..
     START_TEST
     FILENAME: DistributedTest.chpl
     EXECOPTS: --n=10
     START_GOOD
     2 3 4 5 6 7 8 9 10 11
     END_GOOD

  .. code-block:: chapel

      use Python, BlockDist;

      config const n = 100;
      var Arr = blockDist.createArray({1..n}, int);
      Arr = 1..n;

      coforall l in Arr.targetLocales() {
        on l {
          // each locale has its own interpreter
          const interp = new Interpreter();
          const func = new Function(interp, "lambda x,: x + 1");

          var ts = new threadState();
          ts.save();
          forall i in Arr.localSubdomain() with (var gil = new GIL()) {
            Arr[i] = func(Arr.eltType, Arr[i]);
          }
          ts.restore();
        }
      }
      writeln(Arr);

  ..
     END_TEST

  In this example, ``interp`` and ``func`` only exist for the body of the
  ``on`` block,Python objects can be made to persist beyond the scope of a
  given ``on`` block by creating a distributed array of Python objects.

  Defining Custom Types
  ---------------------

  To translate custom Chapel types to Python objects, users should define and
  register custom :type:`TypeConverter` classes.

  More Examples:
  --------------

  For more examples of using the Python module, see the test cases in
  `$CHPL_HOME/test/library/packages/Python/examples <https://github.com/chapel-lang/chapel/tree/main/test/library/packages/Python/examples>`_.

*/
@unstable("The Python module's interface is under active development and may change")
module Python {
  private use CTypes;
  private import List;
  private import Map;

  private use CPythonInterface except PyObject;
  private use CWChar;
  private use OS.POSIX only getenv;

  config const pyMemLeaks = false;

  // TODO: this must be first to avoid use-before-def, but that makes it first in the docs
  // is there a way to avoid this?
  /* Represents the python NoneType */
  record NoneType {
    /*
      Returns the string representation of None.

      Equivalent to calling ``str(None)`` in Python.
    */
    proc str(): string do return "None";
  }
  /* Represents the python value 'None' */
  const None = new NoneType();


  private proc getOsPathSepHelper(interp: borrowed Interpreter): string throws {
    var os = PyImport_ImportModule("os");
    interp.checkException();
    defer Py_DECREF(os);

    var sepPy = PyObject_GetAttrString(os, "pathsep");
    interp.checkException();
    defer Py_DECREF(sepPy);

    var sep = interp.fromPython(string, sepPy);
    return sep;
  }

  /*
    Represents the python interpreter. All code using the Python module should
    create and maintain a single instance of this class.

    .. warning::

       Multiple/sub interpreters are not yet supported.
       Do not create more than one instance of this class.
  */
  class Interpreter {
    @chpldoc.nodoc
    var converters: List.list(owned TypeConverter);
    @chpldoc.nodoc
    var toFree: List.list(PyObjectPtr);
    @chpldoc.nodoc
    var objgraph: PyObjectPtr = nil;

    @chpldoc.nodoc
    proc init() throws {
      init this;

      // preinit
      var preconfig: PyPreConfig;
      PyPreConfig_InitIsolatedConfig(c_ptrTo(preconfig));
      preconfig.utf8_mode = 1;
      checkPyStatus(Py_PreInitialize(c_ptrTo(preconfig)));

      // init
      var config_: PyConfig;
      var cfgPtr = c_ptrTo(config_);
      PyConfig_InitIsolatedConfig(cfgPtr);
      defer PyConfig_Clear(cfgPtr);

      // set program name
      const wideChapel = "chapel".c_wstr();
      checkPyStatus(
        PyConfig_SetString(
          cfgPtr, c_ptrTo(config_.program_name), wideChapel));
      deallocate(wideChapel);

      // check VIRTUAL_ENV, if its set, make it the executable
      var venv = getenv("VIRTUAL_ENV".c_str());
      if venv != nil {
        // ideally this just sets config_.home
        // but by setting executable we can reuse the python logic to determine
        // the locale (in the string sense, not the chapel sense)
        const executable = string.createBorrowingBuffer(venv) + "/bin/python";
        const wideExecutable = executable.c_wstr();
        checkPyStatus(
          PyConfig_SetString(
            cfgPtr, c_ptrTo(config_.executable), wideExecutable));
        deallocate(wideExecutable);
      }

      // initialize
      checkPyStatus(Py_InitializeFromConfig(cfgPtr));

      var sys = PyImport_ImportModule("sys");
      this.checkException();
      defer Py_DECREF(sys);

      // setup sys.path
      {
        var path = PyObject_GetAttrString(sys, "path");
        this.checkException();
        defer Py_DECREF(path);

        // add paths from PYTHONPATH to the front of PATH
        var pythonPathCstr = getenv("PYTHONPATH".c_str());
        if pythonPathCstr != nil {
          const pythonPath = string.createBorrowingBuffer(pythonPathCstr);

          var elms = pythonPath.split(getOsPathSepHelper(this));
          for elm in elms {
            if elm.size == 0 then continue;
            PyList_Append(path, Py_BuildValue("s", elm.c_str()));
            this.checkException();
          }
        }

        // add the current directory to the python path
        PyList_Insert(path, 0, Py_BuildValue("s", "."));
        this.checkException();
      }
      // TODO: reset stdout and stderr to Chapel's handles
      // I think we can do this by setting sys.stdout and sys.stderr to a python
      // object that looks like a python file but forwards calls like write to
      // Chapel's write

      if pyMemLeaks {
        // import objgraph
        this.objgraph = this.importModule("objgraph");
        if this.objgraph == nil {
          writeln("objgraph not found, memory leak detection disabled. Install objgraph with 'pip install objgraph'");
        } else {
          // objgraph.growth()
          var growth = PyObject_GetAttrString(this.objgraph, "growth");
          this.checkException();
          var res = PyObject_CallFunctionObjArgs(growth, Py_None, nil);
          this.checkException();
          Py_DECREF(res);
          Py_DECREF(growth);
        }
      }
    }
    @chpldoc.nodoc
    proc deinit()  {

      for obj in this.toFree {
        Py_CLEAR(c_ptrTo(obj));
      }

      if pyMemLeaks && this.objgraph != nil {
        // note: try! is used since we can't have a throwing deinit

        // run gc.collect() before showing growth
        var gc = PyImport_ImportModule("gc");
        try! this.checkException();
        var collect = PyObject_GetAttrString(gc, "collect");
        try! this.checkException();
        PyObject_CallNoArgs(collect);
        try! this.checkException();
        Py_DECREF(collect);
        Py_DECREF(gc);


        // objgraph.show_growth()
        var show_growth = PyObject_GetAttrString(this.objgraph, "show_growth");
        try! this.checkException();

        PyObject_CallOneArg(show_growth, Py_None);
        try! this.checkException();
        Py_DECREF(show_growth);

        Py_DECREF(this.objgraph);
      }

      Py_Finalize();
    }
    /*
      Run a string of python code.

      This function will just run the code, it cannot be passed arguments or
      return values.
    */
    proc run(code: string) throws {
      PyRun_SimpleString(code.c_str());
      this.checkException();
    }

    /*
      Register a custom :type:`TypeConverter` with the interpreter. This allows
      arbitrary Chapel types to be serialized/deserialized to and from Python
      objects.

      The registered converter will take precedence over the default converters,
      allowing users to override the default behavior.
    */
    proc registerConverter(in cvt: owned TypeConverter) {
      converters.pushBack(cvt);
    }

    /*
      Compile a lambda function like 'lambda x,: x + 1'

      Note: this only works with lambdas that accept a tuple of arguments,
      like 'x,' or '*args'
    */
    @chpldoc.nodoc
    proc compileLambda(l: string): owned PyObject throws {
      var globals = chpl_PyEval_GetFrameGlobals();
      var globalsNeedDecref = false;
      // create a globals if it doesn't exist
      if !globals {
        globals = Py_BuildValue("{}");
        globalsNeedDecref = true;
      }
      // this is the equivalent of `eval(compile(l, '<string>', 'eval'))`
      // we can also do `Py_CompileString` -> `PyFunction_New`?
      var code = PyRun_String(l.c_str(), Py_eval_input, globals, nil);
      this.checkException();
      var obj = new PyObject(this, code);

      if globalsNeedDecref {
        Py_DECREF(globals);
      }
      return obj;
    }

    /*
      Query to see if an Python exception has occurred. If there is an
      exception, the Python exception will be thrown as a :type:`Exception`.

      .. note::

         This method should be called after any Python API call that may fail.
         The wrapping types in this module will call this method automatically,
         most users should not need to call this method directly.
    */
    inline proc checkException() throws {
      var exc = chpl_PyErr_GetRaisedException();
      if exc {
        throw PythonException.build(this, exc);
      }
    }

    @chpldoc.nodoc
    inline proc importModule(in modName: string): PyObjectPtr throws {
      var mod = PyImport_ImportModule(modName.c_str());
      try {
        this.checkException();
      } catch e: ImportError {
        return nil;
      } catch e {
        throw e;
      }
      return mod;
    }

    /*
      Convert a Chapel value to a python object. This clones the Chapel value.

      .. note::

         Most users should not need to call this directly, except when writing
         a :type:`TypeConverter`.
    */
    proc toPython(const val: ?t): PyObjectPtr throws {
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
      } else if isUintType(t) {
        var v = Py_BuildValue("I", val);
        this.checkException();
        return v;
      } else if isRealType(t) {
        var v = PyFloat_FromDouble(val:real);
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
        return val.fn.get();
      } else if isSubtype(t, PyObject) {
        return val.get();
      } else if isSubtype(t, ClassObject) {
        return val.obj.get();
      } else if t == NoneType {
        return Py_None;
      } else {
        halt("Unsupported toPython type: '" + t:string + "'");
      }
    }

    /*
      Convert a Python object to a Chapel value. This clones the Python value.

      .. note::

         Most users should not need to call this directly, except when writing
         a :type:`TypeConverter`.
    */
    proc fromPython(type t, obj: PyObjectPtr): t throws {
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
      } if isUintType(t) {
        var v = PyLong_AsUnsignedLong(obj);
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
        // we need to create a dummy array to determine the type
        pragma "no init"
        var dummy: t;

        if dummy.rank == 1 && dummy.isDefaultRectangular() {
          return fromList(t, obj);
        } else if dummy.isAssociative() {
          return fromDict(t, obj);
        } else {
          halt("Unsupported fromPython array type: '" + t:string + "'");
        }
      } else if isSubtype(t, List.list) {
        return fromList(t, obj);
      } else if isSubtype(t, Function) {
        return new Function("<unknown>", new PyObject(this, obj));
      } else if isSubtype(t, PyObject) || isSubtype(t, PyObject?) {
        return new PyObject(this, obj);
      } else if isSubtype(t, ClassObject) || isSubtype(t, ClassObject?) {
        return new ClassObject(new PyObject(this, obj));
      } else if t == NoneType {
        return None;
      } else {
        halt("Unsupported fromPython type: '" + t:string + "'");
      }
    }

    /*
      Converts an array to a python list
    */
    @chpldoc.nodoc
    proc toList(arr): PyObjectPtr throws
      where isArrayType(arr.type) && arr.rank == 1 && arr.isDefaultRectangular() {
      var pyList = PyList_New(arr.size);
      this.checkException();
      this.toFree.pushBack(pyList);
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
    proc toList(l: List.list(?)): PyObjectPtr throws {
      var pyList = PyList_New(l.size);
      this.checkException();
      this.toFree.pushBack(pyList);
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
    proc fromList(type T, obj: PyObjectPtr): T throws
      where isArrayType(T) {

      if (PySequence_Check(obj) == 0) then
        throw new ChapelException("Can only convert a sequence with a known size to an array");

      // if its a sequence with a known size, we can just iterate over it
      var res: T;
      if PySequence_Size(obj) != res.size {
        throw new ChapelException("Size mismatch");
      }
      for i in 0..<res.size {
        const idx = res.domain.orderToIndex(i);
        var elm = PySequence_GetItem(obj, i);
        this.checkException();
        this.toFree.pushBack(elm);
        res[idx] = fromPython(res.eltType, elm);
        this.checkException();
      }
      return res;
    }

    /*
      Convert a python list to a Chapel list
    */
    @chpldoc.nodoc
    proc fromList(type T, obj: PyObjectPtr): T throws where isSubtype(T, List.list) {

      if (PySequence_Check(obj) != 0) {
        // if it's a sequence with a known size, we can just iterate over it
        var res = new T();
        for i in 0..<PySequence_Size(obj) {
          var item = PySequence_GetItem(obj, i);
          this.checkException();
          this.toFree.pushBack(item);
          res.pushBack(fromPython(T.eltType, item));
        }
        return res;
      } else if (PyIter_Check(obj) != 0 || PyGen_Check(obj) != 0) {
        // if it's an iterator, we can iterate over it
        var res = new T();
        while true {
          var item = PyIter_Next(obj);
          this.checkException();
          this.toFree.pushBack(item);
          if item == nil {
            break;
          }
          res.pushBack(fromPython(T.eltType, item));
        }
        return res;
      } else {
        throw new ChapelException("Expected a Python list or iterable");
      }
    }

    /*
      Converts an array to a python set
    */
    // proc toSet(arr): PyObjectPtr throws {
    // TODO
    // }

    // TODO: convert chapel set to python set

    /*
      Converts a python set to an array
    */
    // proc fromSet(type T, obj: PyObjectPtr): T throws {
    // TODO
    // }

    // TODO: convert python set to chapel set


    /*
      Converts an associative array to a python dictionary
    */
    @chpldoc.nodoc
    proc toDict(arr): PyObjectPtr throws
      where isArrayType(arr.type) && arr.isAssociative() {
      var pyDict = PyDict_New();
      this.checkException();
      this.toFree.pushBack(pyDict);
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
    proc fromDict(type T, obj: PyObjectPtr): T throws
      where isArrayType(T) {

      // no init causes segfaults :(
      var dummy: T;

      // rebuild the array with a modifiable domain
      var dom = dummy.domain;
      var arr: [dom] dummy.eltType;

      type keyType = arr.idxType;
      type valType = arr.eltType;
      var keys = PyDict_Keys(obj);
      defer Py_DECREF(keys);
      this.checkException();
      for i in 0..<PyList_Size(keys) {
        var key = PyList_GetItem(keys, i);
        this.checkException();
        var val = PyDict_GetItem(obj, key);
        this.checkException();

        var keyVal = this.fromPython(keyType, key);
        dom.add(keyVal);
        arr[keyVal] = this.fromPython(valType, val);
      }
      return arr;
    }

    // TODO: convert python dict to chapel map

  }

  /*
    Represents a Python exception, either forwarded from Python (i.e. :proc:`Interpreter.checkException`) or thrown directly in Chapel code.
  */
  class Exception: Error {
    /*
      Creates a new exception with the given message.
    */
    proc init(in message: string) {
      super.init(message);
    }
  }
  /*
    Represents an exception caused in the Python code, forwarded by Chapel code
  */
  class PythonException: Exception {
    proc init(in message: string) {
      super.init(message);
    }
    @chpldoc.nodoc
    proc type build(interp: borrowed Interpreter, exc: PyObjectPtr): owned PythonException throws {
      assert(exc != nil);
      var py_str = PyObject_Str(exc);
      var str = interp.fromPython(string, py_str);
      Py_DECREF(py_str);
      Py_DECREF(exc);
      if PyErr_GivenExceptionMatches(exc, PyExc_ImportError) != 0 {
        return new ImportError(str);
      } else {
        throw new PythonException(str);
      }
    }
  }
  /*
    Represents an ImportError in the Python code
  */
  class ImportError: PythonException {
    proc init(in message: string) {
      super.init(message);
    }
  }
  /*
    Represents an exception caused by code in the Chapel module, not forwarded from Python.
  */
  class ChapelException: Exception {
    proc init(in message: string) {
      super.init(message);
    }
  }

  /*
    Helper error checking function for python initialization functions
  */
  private inline proc checkPyStatus(in status: PyStatus) throws {
    if PyStatus_Exception(status) {
      var str = string.createCopyingBuffer(status.err_msg);
      throw new PythonException(str);
    }
  }


  //
  // TODO: using an interface is the ideal here, but there are too many limitations
  //
  // interface typeConverter {
  //   proc Self.handlesType(type T): bool;
  //   proc Self.toPython(interpreter: borrowed Interpreter,
  //                      type T,
  //                      ref value: T): PyObjectPtr throws;
  //   proc Self.fromPython(interpreter: borrowed Interpreter,
  //                        type T,
  //                        obj: PyObjectPtr): T throws;
  // }

  /*
    Base class for Chapel/Python type converters.

    To create a custom type converter, subclass this class and implement the
    :proc:`~TypeConverter.handlesType`, :proc:`~TypeConverter.toPython`, and
    :proc:`~TypeConverter.fromPython` methods. Then register an instance of this
    class with the interpreter by calling :proc:`~Interpreter.registerConverter`.
  */
  class TypeConverter {
    /*
      Check if the converter can handle the given Chapel type.

      This method should return true if the converter can handle the given type,
      and false otherwise.
    */
    proc handlesType(type T): bool {
      return false;
    }
    /*
      Convert a Chapel value to a Python object.

      This method should convert the Chapel value to a Python object and return
      the :type:`~CTypes.c_ptr` to the underlying Python object.

      The :proc:`~Interpreter.toPython` method will call this method.
    */
    proc toPython(interpreter: borrowed Interpreter,
                  type T, const value: T): PyObjectPtr throws {
      import HaltWrappers;
      HaltWrappers.pureVirtualMethodHalt();
    }
    /*
      Convert a Python object to a Chapel value.

      This method should convert the Python object to a Chapel value and return
      the Chapel value.

      The :proc:`~Interpreter.fromPython` method will call this method.
    */
    proc fromPython(interpreter: borrowed Interpreter,
                    type T, obj: PyObjectPtr): T throws {
      import HaltWrappers;
      HaltWrappers.pureVirtualMethodHalt();
    }
  }

  /*
    Represents a Python module.
  */
  class Module {
    @chpldoc.nodoc
    var modName: string;
    @chpldoc.nodoc
    var mod: owned PyObject;

    /*
      Import a Python module by name.
    */
    proc init(interpreter: borrowed Interpreter, in modName: string) throws {
      this.modName = modName;
      this.mod = new PyObject(interpreter, PyImport_ImportModule(modName.c_str()));
      init this;
      this.mod.check();
    }
    /*
      Get the interpreter associated with this module.
    */
    proc interpreter do return this.mod.interpreter;

    /*
      Returns the string representation of the object.
      This is the same as casting to a string.

      Equivalent to calling ``str(obj)`` in Python.
    */
    proc str(): string throws do return this.mod!.str();

  }

  /*
    Represents a Python function.
  */
  class Function {
    @chpldoc.nodoc
    var fnName: string;
    @chpldoc.nodoc
    var fn_: owned PyObject?;

    @chpldoc.nodoc
    proc fn: borrowed PyObject do return this.fn_!;

    proc init(mod: borrowed Module, in fnName: string) throws {
      this.fnName = fnName;
      this.fn_ = new PyObject(mod.mod.interpreter,
                      PyObject_GetAttrString(mod.mod.get(), fnName.c_str()));
      init this;
      this.fn.check();
    }
    proc init(in fnName: string, in fn: owned PyObject) throws {
      this.fnName = fnName;
      this.fn_ = fn;
      init this;
      this.fn.check();
    }
    proc init(interpreter: borrowed Interpreter, in lambdaFn: string) throws {
      this.fnName = "<anon>";
      init this;
      this.fn_ = interpreter.compileLambda(lambdaFn);
      this.fn.check();
    }
    /*
      Get the interpreter associated with this function.
    */
    proc interpreter do return this.fn.interpreter;

    /*
      Returns the string representation of the object.
      This is the same as casting to a string.

      Equivalent to calling ``str(obj)`` in Python.
    */
    proc str(): string throws do return this.fn.str();

    /*
      Call a python function with Chapel arguments and get a Chapel return value
    */
    proc this(type retType, const args...): retType throws {
      var pyArgs: args.size * PyObjectPtr;
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }

      var pyRes;
      if pyArgs.size == 1 then
        pyRes = PyObject_CallOneArg(this.fn.get(), pyArgs(0));
      else
        pyRes = PyObject_CallFunctionObjArgs(this.fn.get(), (...pyArgs), nil);
      interpreter.checkException();
      interpreter.toFree.pushBack(pyRes);

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }
    proc this(type retType): retType throws {
      var pyRes = PyObject_CallNoArgs(this.fn.get());
      interpreter.checkException();
      interpreter.toFree.pushBack(pyRes);

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }
    pragma "last resort"
    proc this(type retType, const args..., kwargs:?t=none): retType throws
      where kwargs.isAssociative() {
      var pyArgs: args.size * PyObjectPtr;
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

      var pyRes = PyObject_Call(this.fn.get(), pyArg, pyKwargs);
      interpreter.checkException();
      interpreter.toFree.pushBack(pyRes);

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }
    pragma "last resort"
    proc this(type retType, kwargs:?t=none): retType throws
      where kwargs.isAssociative() {

      var pyArg = Py_BuildValue("()");

      var pyKwargs;
      if t != nothing {
        pyKwargs = interpreter.toPython(kwargs);
      } else {
        pyKwargs = nil;
      }

      var pyRes = PyObject_Call(this.fn.get(), pyArg, pyKwargs);
      interpreter.checkException();
      interpreter.toFree.pushBack(pyRes);

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }

    proc getAttr(type t, attr: string): t throws {
      var pyAttr = PyObject_GetAttrString(this.fn.get(), attr.c_str());
      interpreter.checkException();

      var res = interpreter.fromPython(t, pyAttr);
      return res;
    }

  }


  /*
    Represents a Python class.
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
    /*
      Get the interpreter associated with this class.
    */
    proc interpreter do return this.cls.interpreter;

    /*
      Returns the string representation of the object.
      This is the same as casting to a string.

      Equivalent to calling ``str(obj)`` in Python.
    */
    proc str(): string throws do return this.cls!.str();

    /*
      Create a new instance of a python class
    */
    proc newInstance(const args...): owned PyObject throws {
      var pyArgs: args.size * PyObjectPtr;
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
    proc this(const args...): owned ClassObject throws do return new ClassObject(this, (...args));
    proc this(): owned ClassObject throws do return new ClassObject(this);

  }


  /*
    Represents a Python class object.
  */
  class ClassObject {
    @chpldoc.nodoc
    var cls: borrowed Class?;
    @chpldoc.nodoc
    var obj_: owned PyObject?;

    @chpldoc.nodoc
    proc obj: borrowed PyObject do return this.obj_!;

    proc init(cls: borrowed Class, const args...) throws {
      this.cls = cls;
      init this;
      this.obj_ = cls.newInstance((...args));
      this.obj.check();
    }
    proc init(cls: borrowed Class) throws {
      this.cls = cls;
      init this;
      this.obj_ = cls.newInstance();
      this.obj.check();
    }
    proc init(in obj: owned PyObject?) throws {
      this.obj_ = obj;
      init this;
      this.obj.check();
    }
    /*
      Get the interpreter associated with this object.
    */
    proc interpreter do return this.obj.interpreter;

    /*
      Returns the string representation of the object.
      This is the same as casting to a string.

      Equivalent to calling ``str(obj)`` in Python.
    */
    proc str(): string throws do return this.obj.str();


    proc getAttr(type t, attr: string): t throws {
      var pyAttr = PyObject_GetAttrString(this.obj.get(), attr.c_str());
      interpreter.checkException();
      var res = interpreter.fromPython(t, pyAttr);
      return res;
    }
    proc setAttr(attr: string, value) throws {
      var pyValue = interpreter.toPython(value);
      PyObject_SetAttrString(this.obj.get(), attr.c_str(), pyValue);
      interpreter.checkException();
    }

    proc this(type retType, const args...): retType throws {
      var pyArgs: args.size * PyObjectPtr;
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }
      var pyRes;
      if pyArgs.size == 1 then
        pyRes = PyObject_CallOneArg(this.obj.get(), pyArgs(0));
      else
        pyRes = PyObject_CallFunctionObjArgs(this.obj.get(), (...pyArgs), nil);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }

    proc call(type retType, method: string, const args...): retType throws {
      var pyArgs: args.size * PyObjectPtr;
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }

      var methodName = interpreter.toPython(method);
      var pyRes = PyObject_CallMethodObjArgs(
        this.obj.get(), methodName, (...pyArgs), nil);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }
    proc call(type retType, method: string): retType throws {
      var methodName = interpreter.toPython(method);
      var pyRes = PyObject_CallMethodNoArgs(this.obj.get(), methodName);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }
  }

  /*
    Represents the Global Interpreter Lock, this is used to ensure that only one
    thread is executing python code at a time. Each thread should have its own
    :type:`GIL` instance.

    .. warning::

       This is not thread safe, do not attempt to use the same instance from
       multiple threads. This may cause a segfault or deadlock.
  */
  record GIL {
    @chpldoc.nodoc
    var state: PyGILState_STATE;
    @chpldoc.nodoc
    var acquired: bool;
    /*
      Acquires the GIL on record creation by default.
      Set ``acquire`` to false to delay acquisition.
    */
    proc init(param acquire: bool = true) {
      if acquire {
        state = PyGILState_Ensure();
        acquired = true;
      } else {
        acquired = false;
      }
    }
    @chpldoc.nodoc
    proc ref deinit() do this.release();

    /*
      Acquire the GIL. If the GIL is already acquired, this is a no-op.
    */
    proc ref acquire() {
      if !acquired {
        state = PyGILState_Ensure();
        acquired = true;
      }
    }
    /*
      Release the GIL. If the GIL is not acquired, this is a no-op.

      This method is called automatically when the record is destroyed.
    */
    proc ref release() {
      if acquired {
        PyGILState_Release(state);
        acquired = false;
      }
    }
  }

  /*
    Represents the current thread state. This saves and restores the current thread
    state.

    .. warning::

       This is not thread safe, do not attempt to use the same instance from
       multiple threads. This may cause a segfault or deadlock.
  */
  record threadState {
    @chpldoc.nodoc
    var state: PyThreadStatePtr;
    @chpldoc.nodoc
    var saved: bool;
    /*
      Saves the current thread state on record creation by default.
      Set ``save`` to true to save the thread state on object creation.
    */
    proc init(param save: bool = false) {
      if save {
        state = PyEval_SaveThread();
        saved = true;
      } else {
        saved = false;
      }
    }
    @chpldoc.nodoc
    proc ref deinit() do this.restore();

    /*
      Saves the current thread state. If the state is already saved, this is a no-op.
    */
    proc ref save() {
      if !saved {
        state = PyEval_SaveThread();
        saved = true;
      }
    }
    /*
      Restores the thread state. If the state is not saved, this is a no-op.

      This method is called automatically when the record is destroyed.
    */
    proc ref restore() {
      if saved {
        PyEval_RestoreThread(state);
        saved = false;
      }
    }
  }


  /*
    Represents a Python value, it handles reference counting and is owned by default.

    Most users should not need to use this directly.
  */
  class PyObject {
    /*
      The interpreter that this object is associated with.
    */
    var interpreter: borrowed Interpreter;
    @chpldoc.nodoc
    var obj: PyObjectPtr;
    @chpldoc.nodoc
    var isOwned: bool;

    /*
      Takes ownership of an existing Python object, pointed to by ``obj``

      :arg interpreter: The interpreter that this object is associated with.
      :arg obj: The :type:`~CTypes.c_ptr` to the existing object.
      :arg isOwned: Whether this object owns the Python object. This is true by default.
    */
    proc init(in interpreter: borrowed Interpreter, obj: PyObjectPtr, isOwned: bool = true) {
      this.interpreter = interpreter;
      this.obj = obj;
      this.isOwned = isOwned;
    }
    /*
      Creates a new Python object from a Chapel value.

      :arg interpreter: The interpreter that this object is associated with.
      :arg value: The Chapel value to convert to a Python object.
    */
    proc init(in interpreter: borrowed Interpreter, value: ?) {
      this.interpreter = interpreter;
      this.obj = toPython(interpreter, value);
      this.isOwned = true;
    }
    @chpldoc.nodoc
    proc deinit() {
      if this.isOwned then
        Py_DECREF(this.obj);
    }
    proc check() throws do this.interpreter.checkException();
    /*
      Returns the :type:`~CTypes.c_ptr` to the underlying Python object.
    */
    proc get() do return this.obj;

    /*
      Returns the Chapel value of the object.

      This is a shortcut for calling :proc:`~Interpreter.fromPython` on this object.
    */
    proc value(type value) throws {
      return interpreter.fromPython(value, this.obj);
    }

    proc type release(in self: owned PyObject): PyObjectPtr {
      var selfUn: unmanaged PyObject = owned.release(self);
      selfUn.isOwned = false;
      var ptr: PyObjectPtr = selfUn.obj;
      delete selfUn;
      return ptr;
    }
    proc type release(in self: owned ClassObject): PyObjectPtr {
      var pyObj: unmanaged ClassObject = owned.release(self);
      pyObj.obj!.isOwned = false;
      var ptr: PyObjectPtr = pyObj.obj!.get();
      delete pyObj;
      return ptr;
    }


    /*
      Returns the string representation of the object.
      This is the same as casting to a string.

      Equivalent to calling ``str(obj)`` in Python.
    */
    proc str(): string throws {
      var pyStr = PyObject_Str(this.obj);
      interpreter.checkException();
      var res = interpreter.fromPython(string, pyStr);
      return res;
    }
  }

  /*
    Casts a Chapel value to a Python object.
    This is the equivalent of calling ``obj.str()`` in Chapel code.
  */
  @chpldoc.nodoc
  operator:(v, type t: string): string throws
    where isSubtype(v.type, PyObject) || isSubtype(v.type, Module) ||
          isSubtype(v.type, Class) || isSubtype(v.type, Function) ||
          isSubtype(v.type, ClassObject) || isSubtype(v.type, NoneType) {
    return v.str();
  }

  // PyObject intentionally does not have a serialize method
  // its meant to be an implementation detail and not used directly
  Module implements writeSerializable;
  Function implements writeSerializable;
  Class implements writeSerializable;
  ClassObject implements writeSerializable;
  NoneType implements writeSerializable;

  @chpldoc.nodoc
  override proc Module.serialize(writer, ref serializer) throws do
    writer.write(this:string);
  @chpldoc.nodoc
  override proc Function.serialize(writer, ref serializer) throws do
    writer.write(this:string);
  @chpldoc.nodoc
  override proc Class.serialize(writer, ref serializer) throws do
    writer.write(this:string);
  @chpldoc.nodoc
  override proc ClassObject.serialize(writer, ref serializer) throws do
    writer.write(this:string);
  @chpldoc.nodoc
  proc NoneType.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  module CWChar {
    require "wchar.h";
    extern "wchar_t" type c_wchar;
    private use CTypes;

    /*
      Convert a Chapel string to a `wchar_t*`, this is the same as `.c_str()`

      This allocates a new buffer, the caller is responsible for freeing it.
    */
    proc string.c_wstr(): c_ptr(c_wchar) {
      extern proc mbstowcs(dest: c_ptr(c_wchar), src: c_ptrConst(c_char), n: c_size_t): c_size_t;
      var len = this.size;
      var buf = allocate(c_wchar, len + 1, clear=true);
      mbstowcs(buf, this.c_str(), len);
      return buf;
    }
  }

  @chpldoc.nodoc
  module CPythonInterface {
    require "Python.h";
    require "PythonHelper/ChapelPythonHelper.h";
    private use CTypes;
    private use super.CWChar;



    /*
      Types
    */
    extern type PyObject;
    type PyObjectPtr = c_ptr(PyObject);
    extern type PyTypeObject;
    type PyTypeObjectPtr = c_ptr(PyTypeObject);

    extern type PyThreadState;
    type PyThreadStatePtr = c_ptr(PyThreadState);
    extern type PyGILState_STATE;

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
    extern proc PyPreConfig_InitIsolatedConfig(config_: c_ptr(PyPreConfig));
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
    extern proc Py_INCREF(obj: PyObjectPtr);
    extern "chpl_Py_DECREF" proc Py_DECREF(obj: PyObjectPtr);
    extern "chpl_Py_CLEAR" proc Py_CLEAR(obj: c_ptr(PyObjectPtr));
    extern proc PyObject_Str(obj: PyObjectPtr): PyObjectPtr; // `str(obj)`
    extern proc PyImport_ImportModule(name: c_ptrConst(c_char)): PyObjectPtr;

    extern const chpl_PY_VERSION_HEX: uint(64);
    extern const chpl_PY_VERSION: c_ptrConst(c_char);


    /*
      Global exec functions
    */
    extern proc PyRun_SimpleString(code: c_ptrConst(c_char));
    extern proc chpl_PyEval_GetFrameGlobals(): PyObjectPtr;


    extern var Py_eval_input: c_int;
    extern proc PyRun_String(code: c_ptrConst(c_char), start: c_int,
                             globals: PyObjectPtr, locals: PyObjectPtr): PyObjectPtr;

    /*
      Threading
    */
    extern proc PyEval_SaveThread():PyThreadStatePtr;
    extern proc PyEval_RestoreThread(state: PyThreadStatePtr);

    extern proc PyGILState_Ensure(): PyGILState_STATE;
    extern proc PyGILState_Release(state: PyGILState_STATE);

    /*
      Error handling
    */
    extern proc PyErr_Occurred(): PyObjectPtr;
    extern proc chpl_PyErr_GetRaisedException(): PyObjectPtr;
    extern proc PyErr_GivenExceptionMatches(given: PyObjectPtr, exc: PyObjectPtr): c_int;
    extern const PyExc_ImportError: PyObjectPtr;

    /*
      Values
    */
    extern proc Py_BuildValue(format: c_ptrConst(c_char), vals...): PyObjectPtr;
    extern proc Py_BuildValue(format: c_ptrConst(c_char)): PyObjectPtr;
    extern proc PyLong_FromLong(v: c_long): PyObjectPtr;
    extern proc PyLong_AsLong(obj: PyObjectPtr): c_long;
    extern proc PyLong_AsInt(obj: PyObjectPtr): c_int;
    extern proc PyLong_FromUnsignedLong(v: c_ulong): PyObjectPtr;
    extern proc PyLong_AsUnsignedLong(obj: PyObjectPtr): c_ulong;
    extern proc PyFloat_FromDouble(r: real(64)): PyObjectPtr;
    extern proc PyFloat_AsDouble(obj: PyObjectPtr): real(64);
    extern proc PyBool_FromLong(v: c_long): PyObjectPtr;
    extern proc PyString_FromString(s: c_ptrConst(c_char)): PyObjectPtr;
    extern proc PyUnicode_AsUTF8(obj: PyObjectPtr): c_ptrConst(c_char);

    proc Py_None: PyObjectPtr {
      extern proc chpl_Py_None(): PyObjectPtr;
      return chpl_Py_None();
    }
    proc Py_True: PyObjectPtr {
      extern proc chpl_Py_True(): PyObjectPtr;
      return chpl_Py_True();
    }
    proc Py_False: PyObjectPtr {
      extern proc chpl_Py_False(): PyObjectPtr;
      return chpl_Py_False();
    }

    /*
      Sequences
    */
    extern proc PySequence_Check(obj: PyObjectPtr): c_int;
    extern proc PySequence_Size(obj: PyObjectPtr): c_long;
    extern proc PySequence_GetItem(obj: PyObjectPtr, idx: c_long): PyObjectPtr;
    extern proc PySequence_SetItem(obj: PyObjectPtr, idx: c_long, value: PyObjectPtr);


    /*
      Iterators
    */
    extern "chpl_PyList_Check" proc PyIter_Check(obj: PyObjectPtr): c_int;
    extern "chpl_PyGen_Check" proc PyGen_Check(obj: PyObjectPtr): c_int;
    extern proc PyIter_Next(obj: PyObjectPtr): PyObjectPtr;

    /*
      Lists
    */
    extern proc PyList_Check(obj: PyObjectPtr): c_int;
    extern proc PyList_New(size: c_long): PyObjectPtr;
    extern proc PyList_SetItem(list: PyObjectPtr, idx: c_long, item: PyObjectPtr);
    extern proc PyList_GetItem(list: PyObjectPtr, idx: c_long): PyObjectPtr;
    extern proc PyList_Size(list: PyObjectPtr): c_long;
    extern proc PyList_Insert(list: PyObjectPtr, idx: c_long, item: PyObjectPtr);
    extern proc PyList_Append(list: PyObjectPtr, item: PyObjectPtr);

    /*
      Sets
    */
    extern proc PySet_New(): PyObjectPtr;
    extern proc PySet_Add(set: PyObjectPtr, item: PyObjectPtr);
    extern proc PySet_Size(set: PyObjectPtr): c_long;


    /*
      Dictionaries
    */
    extern proc PyDict_New(): PyObjectPtr;
    extern proc PyDict_SetItem(dict: PyObjectPtr, key: PyObjectPtr, value: PyObjectPtr);
    extern proc PyDict_SetItemString(dict: PyObjectPtr, key: c_ptrConst(c_char),
                                     value: PyObjectPtr);
    extern proc PyDict_GetItem(dict: PyObjectPtr, key: PyObjectPtr): PyObjectPtr;
    extern proc PyDict_GetItemString(dict: PyObjectPtr,
                                     key: c_ptrConst(c_char)): PyObjectPtr;
    extern proc PyDict_Size(dict: PyObjectPtr): c_long;
    extern proc PyDict_Keys(dict: PyObjectPtr): PyObjectPtr;

    extern proc PyObject_GetAttrString(obj: PyObjectPtr,
                                       name: c_ptrConst(c_char)): PyObjectPtr;
    extern proc PyObject_SetAttrString(obj: PyObjectPtr,
                                       name: c_ptrConst(c_char),
                                       value: PyObjectPtr);

    /*
      Tuples
    */
    extern proc PyTuple_Pack(size: c_long, args...): PyObjectPtr;

    /*
      Functions
    */
    extern proc PyObject_Call(obj: PyObjectPtr, args: PyObjectPtr, kwargs: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallObject(obj: PyObjectPtr, args: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallNoArgs(obj: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallOneArg(obj: PyObjectPtr, arg: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallFunctionObjArgs(obj: PyObjectPtr, args...): PyObjectPtr;

    extern proc PyObject_CallMethodNoArgs(obj: PyObjectPtr,
                                          method: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallMethodObjArgs(obj: PyObjectPtr,
                                           method: PyObjectPtr, args...): PyObjectPtr;

  }
}
