/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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


//  TODO: represent python context managers as Chapel context managers

// TODO: implement operators as dunder methods

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

  .. warning::

    Chapel programs compiled in this was are compiled for a specific Python
    version. Attempting to run the compiled program with a different Python
    version may have unexpected results.

  Parallel Execution
  ------------------

  Running any Python code in parallel from Chapel requires special care, due
  to the Global Interpreter Lock (GIL) in the Python interpreter.
  This module supports two ways of doing this.

  .. note::

     Newer Python versions offer a free-threading mode that allows multiple
     threads concurrently. This currently requires a custom build of Python. If
     you are using a Python like this, you should be able to use this module
     freely in parallel code.

  Using Multiple Interpreters
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  .. warning::

     Sub-interpreter support in Chapel is highly experimental and currently has
     undefined behavior.

  The most performant way to run Python code in parallel is to use multiple
  sub-interpreters. Each sub-interpreter is isolated from the others with its
  own GIL. This allows multiple threads to run Python code concurrently. Note
  that communication between sub-interpreters is severely limited and it is
  strongly recommend to limit the amount of data shared between
  sub-interpreters.

  .. note::

     This feature is only available in Python 3.12 and later. Attempting to use
     sub-interpreters with earlier versions of Python will result in a runtime
     exception.

  The following demonstrates using sub-interpreters in a ``coforall`` loop:

  ..
     START_TEST
     FILENAME: CoforallTestSub.chpl
     NOEXEC
     START_GOOD
     END_GOOD

  .. code-block:: chapel

     use Python;

     var code = """
     import sys
     def hello():
       print('Hello from a task')
       sys.stdout.flush()
     """;

     proc main() {
       var interpreter = new Interpreter();
       coforall 0..#4 {
         var subInterpreter = new SubInterpreter(interpreter);
         var m = new Module(subInterpreter, 'myMod', code);
         var hello = new Function(m, 'hello');
         hello(NoneType);
       }
     }

  ..
     END_TEST

  To make use of a sub-interpreter in a ``forall`` loop, the sub-interpreter
  should be created as a task private variable. It is recommended that users
  wrap the sub-interpreter in a ``record`` to initialize their Python objects,
  to prevent duplicated work. For example, the following code creates multiple
  sub-interpreters in a ``forall`` loop, where each task gets its own copy of
  the module.

  ..
     START_TEST
     FILENAME: TaskPrivateSubInterp.chpl
     NOEXEC
     START_GOOD
     END_GOOD

  .. code-block:: chapel

     use Python;

     record perTaskModule {
      var i: owned SubInterpreter?;
      var m: owned Module?;
      proc init(parent: borrowed Interpreter, code: string) {
        init this;
        i = try! (new SubInterpreter(parent));
        m = try! (new Module(i!, "anon", code));
      }
     }

     proc main() {
       var interpreter = new Interpreter();
       forall i in 1..10
        with (var mod =
          new perTaskModule(interpreter, "x = 10")) {
          writeln(mod.m!.getAttr(int, "x"));
       }
     }

  ..
     END_TEST

  Using A Single Interpreter
  ~~~~~~~~~~~~~~~~~~~~~~~~~~

  A single Python interpreter can execute code in parallel, as long as the GIL
  is properly handled. Before any parallel execution with Python code can occur,
  the thread state needs to be saved. After the parallel execution, the thread
  state must to be restored. Then for each thread, the GIL must be acquired and
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

  Using Python Modules With Distributed Code
  -------------------------------------------

  Python has no built-in support for distributed memory, so each locale must
  create its own interpreter (and subsequent Python objects).

  The following example demonstrates how to create a Python interpreter and run
  a Python function on each locale:

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

  Notes on Python + Chapel I/O
  ----------------------------

  When interspersing Python and Chapel I/O, it is important to flush the output
  buffers to ensure that the output is displayed in the correct order. This is
  needed at the point where the output changes from Python to Chapel or
  vice-versa. For example:

  ..
     START_TEST
     FILENAME: Printing.chpl
     START_GOOD
     Hello from Chapel
     Let's call some Python!
     Hello, World!
     Goodbye, World!
     Back to Chapel
     END_GOOD

  .. code-block:: chapel

     use Python, IO;

     var interp = new Interpreter();
     var func = new Function(interp, "lambda x,: print(x)");

     writeln("Hello from Chapel");
     writeln("Let's call some Python!");
     IO.stdout.flush(); // flush the Chapel output buffer before calling Python

     func(NoneType, "Hello, World!");
     func(NoneType, "Goodbye, World!");
     interp.flush(); // flush the Python output buffer before calling Chapel again

     writeln("Back to Chapel");

  ..
     END_TEST


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
  private import this.ArrayTypes;
  private use CWChar;
  private use OS.POSIX only getenv;

  /*
    Use 'objgraph' to detect memory leaks in the Python code. Care should be
    taken when interpreting the output of this flag, not all memory leaks are
    under Chapel's control. For example, printing a Python list leaks memory
    according to 'objgraph'. Furthermore, some memory is still held until
    the interpreter is closed, like the module import cache.
  */
  config const pyMemLeaks = false;

  /*
    Check for exceptions after each Python API call. This is important for
    correctness, but may have a performance impact.

    .. warning::

       While turning this flag off may improve performance, it may also lead to
       segmentation faults and other undefined behavior. This flag should only
       be turned off if you are certain that no exceptions will be thrown by
       the Python code, or if a hard crash is acceptable.
  */
  config param checkExceptions = true;

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

    var sep = interp.fromPython(string, sepPy);
    return sep;
  }

  private proc throwChapelException(message: string) throws {
    throw new ChapelException(message);
  }

  /*
    Represents the python interpreter. All code using the Python module should
    create and maintain a single instance of this class.

    .. warning::

       Do not create more than one instance of this class per locale. Multiple
       interpreters can be created by using :type:`SubInterpreter` instances.
  */
  class Interpreter {

    // TODO: add ability to override the global checkExceptions
    // currently blocked by https://github.com/chapel-lang/chapel/issues/26579
    /*
      Whether to check for exceptions after each Python API call. This is
      important for correctness, but may have a performance impact.

      See :config:`Python.checkExceptions` and
      :proc:`Interpreter.checkException` for more information.
    */
    // param checkExceptions: bool = Python.checkExceptions;

    @chpldoc.nodoc
    var converters: List.list(owned TypeConverter);
    @chpldoc.nodoc
    var objgraph: PyObjectPtr = nil;
    @chpldoc.nodoc
    var isSubInterpreter: bool;

    @chpldoc.nodoc
    proc init(isSubInterpreter: bool = false) {
      this.isSubInterpreter = isSubInterpreter;
      init this;
    }
    @chpldoc.nodoc
    proc postinit() throws {
      if this.isSubInterpreter then return;

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

      if !ArrayTypes.createArrayTypes() {
        throwChapelException("Failed to create Python array types for Chapel arrays");
      }
      if !ArrayTypes.registerArrayTypeEnum() {
        throwChapelException("Failed to register Python array types for Chapel arrays");
      }

      if pyMemLeaks {
        // import objgraph
        this.objgraph = this.importModule("objgraph");
        if this.objgraph == nil {
          writeln("objgraph not found, memory leak detection disabled. " +
                  "Install objgraph with 'pip install objgraph'");
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
      if this.isSubInterpreter then return;

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

      Creates a new reference
    */
    @chpldoc.nodoc
    proc compileLambda(l: string): PyObjectPtr throws {
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

      if globalsNeedDecref {
        Py_DECREF(globals);
      }
      return code;
    }

    /*
      Compile a chunk of python code as a module and return the module object.

      This is equivalent to running the code in a file, and then importing the
      module.

      Creates a new reference
    */
    @chpldoc.nodoc
    proc compileModule(s: string, modname: string = "chplmod"): PyObjectPtr throws
    {
      var code = Py_CompileString(s.c_str(), "<string>", Py_file_input);
      this.checkException();
      defer Py_DECREF(code);

      var mod = PyImport_ExecCodeModule(modname.c_str(), code);
      this.checkException();
      return mod;
    }

    /*
      Interpret a chunk of Python bytecode as a Python module and return the
      module object.

      This is equivalent to running the code in a file, and then importing the
      module. However, this takes the bytecode directly (i.e. *.pyc files),
      rather than the source.

      Creates a new reference
    */
    @chpldoc.nodoc
    proc compileModule(b: bytes,
                       modname: string = "chplmod"): PyObjectPtr throws {
      var code =
        PyMarshal_ReadObjectFromString(b.c_str(), b.size.safeCast(Py_ssize_t));
      this.checkException();
      defer Py_DECREF(code);

      var mod = PyImport_ExecCodeModule(modname.c_str(), code);
      this.checkException();
      return mod;
    }

    /*
      Load a pickle file as a Python object

      Creates a new reference
    */
    @chpldoc.nodoc
    proc loadPickle(b: bytes): PyObjectPtr throws {
      var pickle = importModule("pickle");
      this.checkException();
      defer Py_DECREF(pickle);
      var loads = PyObject_GetAttrString(pickle, "loads");
      this.checkException();
      defer Py_DECREF(loads);
      var pyBytes = toPython(b);
      defer Py_DECREF(pyBytes);
      var obj = PyObject_CallOneArg(loads, pyBytes);
      this.checkException();
      return obj;
    }

    /*
      Load a .pyc file conforming to the Python 3.7+ pyc format as defined in
      <https://peps.python.org/pep-0552/>.
    */
    @chpldoc.nodoc
    proc loadPycFile(filename: string): bytes throws {
      use IO;
      var r = openReader(filename);
      // pyc files have 16 bytes of metadata at the start
      var header = r.readBytes(16);
      // the first 4 bytes are the magic number
      // we validate it against 'importlib.util.MAGIC_NUMBER'
      var magic = header[0..#4];
      var importlib_util = importModule("importlib.util");
      this.checkException();
      defer Py_DECREF(importlib_util);
      var MAGIC_NUMBER_py =
        PyObject_GetAttrString(importlib_util, "MAGIC_NUMBER");
      this.checkException();
      var MAGIC_NUMBER = this.fromPython(bytes, MAGIC_NUMBER_py);
      if magic != MAGIC_NUMBER {
        throw new ChapelException("Invalid magic number in '.pyc' file");
      }

      // the second 4 bytes are a bitfield that determine the rest of the
      // header format. if the bitfield is 0, the rest of the header is a
      // 4 byte timestamp and 4 byte file size. if the lowest bit is set, then
      // the rest of the header is a hash of the file.
      // for the purposes of this function, we ignore all of this

      // the rest of the file is the bytecode
      var modBytes = r.readAll(bytes);
      return modBytes;
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
      if Python.checkExceptions {
        var exc = chpl_PyErr_GetRaisedException();
        if exc then throw PythonException.build(this, exc);
      }
    }

    /*
      Flush the standard output buffers of the Python interpreter. This is
      useful when mixing Python and Chapel I/O to ensure that the output is
      displayed in the correct order.
    */
    inline proc flush(flushStderr: bool = false) throws {
      var stdout = PySys_GetObject("stdout");
      if stdout == nil then throw new ChapelException("stdout not found");

      var flushStr = this.toPython("flush");
      defer Py_DECREF(flushStr);

      PyObject_CallMethodNoArgs(stdout, flushStr);
      if flushStderr {
        var stderr = PySys_GetObject("stderr");
        if stderr == nil then throw new ChapelException("stderr not found");
        PyObject_CallMethodNoArgs(stderr, flushStr);
      }
    }

    @chpldoc.nodoc
    inline proc importModule(in modName: string): PyObjectPtr throws {
      var mod = PyImport_ImportModule(modName.c_str());
      try {
        this.checkException();
      } catch e: ImportError {
        return nil;
      }
      return mod;
    }

    /*
      Convert a Chapel value to a python object. This clones the Chapel value.

      This returns a new reference to a Python object.

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
        var v = PyBool_FromLong(val.safeCast(c_long));
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
      } else if t == bytes {
        var v =
          PyBytes_FromStringAndSize(val.c_str(), val.size.safeCast(Py_ssize_t));
        this.checkException();
        return v;
      } else if isArrayType(t) && val.rank == 1 && val.isDefaultRectangular(){
        return toList(val);
      } else if isArrayType(t) && val.isAssociative() {
        return toDict(val);
      } else if isSubtype(t, List.list) {
        return toList(val);
      } else if isSubtype(t, Value) {
        Py_INCREF(val.get());
        return val.get();
      } else if t == NoneType {
        return Py_None;
      } else {
        halt("Unsupported toPython type: '" + t:string + "'");
      }
    }

    /*
      Convert a Python object to a Chapel value. This clones the Python value.

      This steals a reference to the Python object, so the Chapel object will
      either own the Python object or it will decrement the reference count
      when it is done with it.

      .. note::

         Most users should not need to call this directly, except when writing
         a :type:`TypeConverter`.
    */
    proc fromPython(type t, obj: PyObjectPtr): t throws {
      if isClassType(t) &&
         (!isSharedClassType(t) &&
          !isOwnedClassType(t) &&
          !isUnmanagedClassType(t)) {
        compilerError("fromPython only supports shared, owned, and unmanaged classes");
      }
      if isSubtype(t, Array?) {
        compilerError("Cannot create an Array from an existing PyObject");
      }
      if isSubtype(t, PyArray?) {
        if isGeneric(t) {
          compilerError("Cannot get a generic PyArray, try specifying the eltType like 'PyArray(int)'");
        }
        return new t(this, obj);
      }

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
      } else if t == string {
        var v = string.createCopyingBuffer(PyUnicode_AsUTF8(obj));
        Py_DECREF(obj);
        this.checkException();
        return v;
      } else if t == bytes {
        var v = bytes.createCopyingBuffer(PyBytes_AsString(obj),
                                          PyBytes_Size(obj).safeCast(int));
        this.checkException();
        Py_DECREF(obj);
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
      } else if isSubtype(t, Function?) {
        return new t(this, "<unknown>", obj);
      } else if isSubtype(t, Value?) {
        return new t(this, obj);
      } else if t == NoneType {
        // returning NoneType can be used to ignore a return value
        // but if its not actually None, we still need to decrement the reference count
        if obj != Py_None then Py_DECREF(obj);
        return None;
      } else {
        halt("Unsupported fromPython type: '" + t:string + "'");
      }
    }

    /*
      Converts an array to a python list. Returns new reference
    */
    @chpldoc.nodoc
    proc toList(arr): PyObjectPtr throws
      where isArrayType(arr.type) &&
            arr.rank == 1 && arr.isDefaultRectangular() {
      var pyList = PyList_New(arr.size.safeCast(Py_ssize_t));
      this.checkException();
      for i in 0..<arr.size {
        const idx = arr.domain.orderToIndex(i);
        PyList_SetItem(pyList, i.safeCast(Py_ssize_t), toPython(arr[idx]));
        this.checkException();
      }
      return pyList;
    }

    /*
      Convert a chapel list to a python list. Returns a new reference
    */
    @chpldoc.nodoc
    proc toList(l: List.list(?)): PyObjectPtr throws {
      var pyList = PyList_New(l.size.safeCast(Py_ssize_t));
      this.checkException();
      for i in 0..<l.size {
        PyList_SetItem(pyList, i.safeCast(Py_ssize_t), toPython(l(i)));
        this.checkException();
      }
      return pyList;
    }

    /*
      Converts a python list to an array. Steals a reference to obj.
    */
    @chpldoc.nodoc
    proc fromList(type T, obj: PyObjectPtr): T throws
      where isArrayType(T) {

      if (PySequence_Check(obj) == 0) then
        throw new ChapelException("Can only convert a sequence with a known size to an array");

      // if its a sequence with a known size, we can just iterate over it
      var res: T;
      if PySequence_Size(obj) != res.size.safeCast(Py_ssize_t) {
        throw new ChapelException("Size mismatch");
      }
      for i in 0..<res.size {
        const idx = res.domain.orderToIndex(i);
        var elm = PySequence_GetItem(obj, i.safeCast(Py_ssize_t));
        this.checkException();
        res[idx] = fromPython(res.eltType, elm);
        this.checkException();
      }
      Py_DECREF(obj);
      return res;
    }

    /*
      Convert a python list to a Chapel list. Steals a reference to obj.
    */
    @chpldoc.nodoc
    proc fromList(type T, obj: PyObjectPtr): T throws where isSubtype(T, List.list) {

      if PySequence_Check(obj) != 0 {
        // if it's a sequence with a known size, we can just iterate over it
        var res = new T();
        for i in 0..<PySequence_Size(obj) {
          var item = PySequence_GetItem(obj, i);
          this.checkException();
          res.pushBack(fromPython(T.eltType, item));
        }
        Py_DECREF(obj);
        return res;
      } else if PyIter_Check(obj) != 0 || PyGen_Check(obj) != 0 {
        // if it's an iterator, we can iterate over it
        var res = new T();
        while true {
          var item = PyIter_Next(obj);
          this.checkException();
          if item == nil {
            break;
          }
          res.pushBack(fromPython(T.eltType, item));
        }
        Py_DECREF(obj);
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
      Converts an associative array to a python dictionary.
      Returns a new reference.
    */
    @chpldoc.nodoc
    proc toDict(arr): PyObjectPtr throws
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
      Converts a python dictionary to an associative array.
      Steals a reference to obj.
    */
    proc fromDict(type T, obj: PyObjectPtr): T throws
      where isArrayType(T) {

      // rebuild the array with a modifiable domain
      var dom = chpl__domainFromArrayRuntimeType(T);
      type eltType = chpl__eltTypeFromArrayRuntimeType(T);
      var arr: [dom] eltType;

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

      Py_DECREF(obj);
      return arr;
    }

    // TODO: convert python dict to chapel map

  }

  /*
    Represents an isolated Python sub-interpreter. This is useful for running
    truly parallel Python code, without the GIL interferring.
  */
  class SubInterpreter: Interpreter {
    @chpldoc.nodoc
    var parent: borrowed Interpreter;
    @chpldoc.nodoc
    var tstate: PyThreadStatePtr;

    /*
      Creates a new sub-interpreter with the given parent interpreter, which
      must not be a sub-interpreter.
    */
    proc init(parent: borrowed Interpreter) {
      super.init(isSubInterpreter=true);
      this.parent = parent;
      init this;
    }
    @chpldoc.nodoc
    proc postinit() throws {
      if this.parent.isSubInterpreter {
        throwChapelException("Parent interpreter cannot be a sub-interpreter");
      }

      checkPyStatus(chpl_Py_NewIsolatedInterpreter(c_ptrTo(this.tstate)));
    }
    @chpldoc.nodoc
    proc deinit() {
      Py_EndInterpreter(this.tstate);
    }
  }

  /*
    Represents a Python exception, either forwarded from Python
    (i.e. :proc:`Interpreter.checkException`) or thrown directly in Chapel code.
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
    proc type build(interp: borrowed Interpreter,
                    exc: PyObjectPtr): owned PythonException throws {
      assert(exc != nil);
      var py_str = PyObject_Str(exc);
      var str = interp.fromPython(string, py_str);
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
    Represents a BufferError in the Python code
  */
  class BufferError: PythonException {
    proc init(in message: string) {
      super.init(message);
    }
  }

  /*
    Represents an exception caused by code in the Chapel module,
    not forwarded from Python.
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
  // TODO: using an interface is the ideal, but there are too many limitations
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
    class with the interpreter by calling
    :proc:`~Interpreter.registerConverter`.
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
    Represents a Python value, it handles reference counting and is owned by default.
  */
  class Value {
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
      init this;
    }

    /*
      Creates a new Python object from a pickle.

      :arg interpreter: The interpreter that this object is associated with.
      :arg pickleData: The pickle data to load.
    */
    proc init(in interpreter: borrowed Interpreter, pickleData: bytes) throws {
      this.interpreter = interpreter;
      this.isOwned = true;
      init this;
      this.obj = this.interpreter.loadPickle(pickleData);
    }
    /*
      Creates a new Python object from a Chapel value.

      :arg interpreter: The interpreter that this object is associated with.
      :arg value: The Chapel value to convert to a Python object.
    */
    proc init(in interpreter: borrowed Interpreter, value: ?) throws {
      this.interpreter = interpreter;
      this.isOwned = true;
      init this;
      this.obj = this.interpreter.toPython(value);
    }

    @chpldoc.nodoc
    proc postinit() throws {
      this.check();
    }

    @chpldoc.nodoc
    proc deinit() {
      if this.isOwned then
        Py_DECREF(this.obj);
    }

    inline proc check() throws do this.interpreter.checkException();

    /*
      Returns the :type:`~CTypes.c_ptr` to the underlying Python object.
    */
    proc get() do return this.obj;

    /*
      Returns the Chapel value of the object.

      This is a shortcut for calling :proc:`~Interpreter.fromPython` on this
      object, however it does not consume the object.
    */
    proc value(type value) throws {
      // fromPython will decrement the reference count, so we need to increment it
      Py_INCREF(this.obj);
      return interpreter.fromPython(value, this.obj);
    }

    /*
      Stop owning val, return the underlying ptr.
    */
    proc type release(in val: owned Value): PyObjectPtr {
      var valUn: unmanaged = owned.release(val);
      valUn.isOwned = false;
      var ptr: PyObjectPtr = valUn.obj;
      delete valUn;
      return ptr;
    }
    /*
      Create a new Value, taking ownership of the object.
    */
    proc type adopting(in interpreter: borrowed Interpreter, in ptr: PyObjectPtr): owned Value throws {
      var val = new Value(interpreter, ptr, isOwned=true);
      return val;
    }
    /*
      Create a new Value, borrowing the object.
    */
    proc type borrowing(in interpreter: borrowed Interpreter, in ptr: PyObjectPtr): owned Value throws {
      var val = new Value(interpreter, ptr, isOwned=false);
      return val;
    }


    /*
      Returns the string representation of the object.
      This is the same as casting to a string.

      Equivalent to calling ``str(obj)`` in Python.
    */
    proc str(): string throws {
      var pyStr = PyObject_Str(this.obj);
      this.check();
      var res = interpreter.fromPython(string, pyStr);
      return res;
    }

    /*
      Treat this value as a callable and call it with the given arguments.

      This handles conversion from Chapel types to Python types and back, by
      copying the Chapel types to Python types and back.

      The keyword arguments should be passed as an associative Chapel array.
      For example:

      .. code-block:: chapel

         var res = myObj(int, arg1, arg2, kwargs=["key1" => 42, "key2" => 17]);

      :arg retType: The Chapel type of the return value. If the callable returns
                    nothing, use :type:`NoneType`.
      :arg args: The arguments to pass to the callable.
      :arg kwargs: The keyword arguments to pass to the callable.
    */
    pragma "last resort"
    proc this(type retType,
              const args...,
              kwargs:?=none): retType throws
              where kwargs.isAssociative() {
      var pyArg = this.packTuple((...args));
      defer Py_DECREF(pyArg);
      return callInternal(retType, pyArg, kwargs);
    }
    pragma "last resort"
    @chpldoc.nodoc
    proc this(type retType,
              kwargs:?=none): retType throws where kwargs.isAssociative() {
      var pyArgs = Py_BuildValue("()");
      defer Py_DECREF(pyArgs);
      return callInternal(retType, pyArgs, kwargs);
    }
    @chpldoc.nodoc
    proc this(type retType, const args...): retType throws {
      var pyArg = this.packTuple((...args));
      defer Py_DECREF(pyArg);
      return callInternal(retType, pyArg, none);
    }
    @chpldoc.nodoc
    proc this(type retType): retType throws {
      var pyRes = PyObject_CallNoArgs(this.get());
      this.check();

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }

    @chpldoc.nodoc
    proc packTuple(const args...) throws {
      var pyArgs: args.size * PyObjectPtr;
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }
      defer for pyArg in pyArgs do Py_DECREF(pyArg);

      var pyArg = PyTuple_Pack(args.size.safeCast(Py_ssize_t), (...pyArgs));
      interpreter.checkException();

      return pyArg;
    }
    @chpldoc.nodoc
    proc callInternal(type retType,
                      pyArg: PyObjectPtr, kwargs: ?t): retType throws {
      var pyKwargs;
      if t != nothing then pyKwargs = interpreter.toPython(kwargs);
                      else pyKwargs = nil;

      var pyRes = PyObject_Call(this.obj, pyArg, pyKwargs);
      this.check();

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }

    /*
      Access an attribute/field of this Python object. This is equivalent to
      calling ``obj[attr]`` or ``getattr(obj, attr)`` in Python.

      :arg t: The Chapel type of the value to return.
      :arg attr: The name of the attribute/field to access.
    */
    proc getAttr(type t, attr: string): t throws {
      var pyAttr = PyObject_GetAttrString(this.get(), attr.c_str());
      interpreter.checkException();

      var res = interpreter.fromPython(t, pyAttr);
      return res;
    }

    /*
      Set an attribute/field of this Python object. This is equivalent to
      calling ``obj[attr] = value`` or ``setattr(obj, attr, value)`` in Python.

      :arg attr: The name of the attribute/field to set.
      :arg value: The value to set the attribute/field to.
    */
    proc setAttr(attr: string, value) throws {
      var pyValue = interpreter.toPython(value);
      defer Py_DECREF(pyValue);

      PyObject_SetAttrString(this.get(), attr.c_str(), pyValue);
      interpreter.checkException();
    }

    /*
      Call a method of this Python object. This is equivalent to calling
      ``obj.method(args)`` in Python.

      :arg retType: The Chapel type of the return value.
      :arg method: The name of the method to call.
      :arg args: The arguments to pass to the method.
    */
    proc call(type retType, method: string, const args...): retType throws {
      var pyArgs: args.size * PyObjectPtr;
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPython(args(i));
      }
      defer for pyArg in pyArgs do Py_DECREF(pyArg);

      var methodName = interpreter.toPython(method);
      defer Py_DECREF(methodName);

      var pyRes = PyObject_CallMethodObjArgs(
        this.get(), methodName, (...pyArgs), nil);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }
    @chpldoc.nodoc
    proc call(type retType, method: string): retType throws {
      var methodName = interpreter.toPython(method);
      defer Py_DECREF(methodName);

      var pyRes = PyObject_CallMethodNoArgs(this.get(), methodName);
      interpreter.checkException();

      var res = interpreter.fromPython(retType, pyRes);
      return res;
    }
  }

  /*
    Represents a Python module.
  */
  class Module: Value {
    @chpldoc.nodoc
    var modName: string;

    /*
      Import a Python module by name.
    */
    proc init(interpreter: borrowed Interpreter, in modName: string) {
      super.init(interpreter,
                 PyImport_ImportModule(modName.c_str()), isOwned=true);
      this.modName = modName;
    }

    /*
      Import a Python module from a string using an arbitrary name.
    */
    proc init(interpreter: borrowed Interpreter,
              in modName: string, in moduleContents: string) throws {
      super.init(interpreter, nil: PyObjectPtr, isOwned=true);
      this.modName = modName;
      init this;
      this.obj = interpreter.compileModule(moduleContents, modName);
    }

    /*
      Import a Python module from a string using an arbitrary name.
    */
    proc init(interpreter: borrowed Interpreter,
              in modName: string, in moduleBytecode: bytes) throws {
      super.init(interpreter, nil: PyObjectPtr, isOwned=true);
      this.modName = modName;
      init this;
      this.obj = interpreter.compileModule(moduleBytecode, modName);
    }
  }

  /*
    Represents a Python function.
  */
  class Function: Value {
    @chpldoc.nodoc
    var fnName: string;

    /*
      Get a handle to a function in a :class:`Module` by name.
    */
    proc init(mod: borrowed Value, in fnName: string) {
      super.init(mod.interpreter,
                 PyObject_GetAttrString(mod.get(), fnName.c_str()),
                 isOwned=true);
      this.fnName = fnName;
    }
    /*
      Takes ownership of an existing Python object, pointed to by ``obj``

      :arg interpreter: The interpreter that this object is associated with.
      :arg fnName: The name of the function.
      :arg obj: The :type:`~CTypes.c_ptr` to the existing object.
      :arg isOwned: Whether this object owns the Python object. This is true by default.
    */
    proc init(interpreter: borrowed Interpreter,
              in fnName: string, in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
      this.fnName = fnName;
    }
    /*
      Create a new Python lambda function from a string. The lambda arguments must
      have a trailing comma.

      For example, to create a lambda function that takes two arguments, use:

      .. code-block:: python

         new Function(interpreter, "lambda x, y,: x + y")

      :arg interpreter: The interpreter that this object is associated with.
      :arg lambdaFn: The lambda function to create.
    */
    proc init(interpreter: borrowed Interpreter, in lambdaFn: string) throws {
      super.init(interpreter, nil: PyObjectPtr, isOwned=true);
      this.fnName = "<anon>";
      init this;
      this.obj = interpreter.compileLambda(lambdaFn);
    }
    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
      this.fnName = "<unknown>";
    }
  }


  /*
    Represents a Python class.
  */
  class Class: Value {
    @chpldoc.nodoc
    var className: string;

    /*
      Get a handle to a class in a :class:`Module` by name.

      :arg mod: The module to get the class from.
      :arg className: The name of the class.
    */
    proc init(mod: borrowed Value, in className: string) {
      super.init(mod.interpreter,
                 PyObject_GetAttrString(mod.get(), className.c_str()),
                 isOwned=true);
      this.className = className;
    }

    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
    }

    @chpldoc.nodoc
    proc newInstance(const args...): PyObjectPtr throws {
      var pyArg = packTuple((...args));
      defer Py_DECREF(pyArg);
      var pyRes = PyObject_Call(this.get(), pyArg, nil);
      this.check();
      return pyRes;
    }
    @chpldoc.nodoc
    proc newInstance(): PyObjectPtr throws {
      var pyRes = PyObject_CallNoArgs(this.get());
      this.check();
      return pyRes;
    }

    /*
      Create a new instance of a Python class
    */
    proc this(const args...): owned Value throws do
      return new Value(interpreter, newInstance((...args)), isOwned=true);
    @chpldoc.nodoc
    proc this(): owned Value throws do
      return new Value(interpreter, newInstance(), isOwned=true);

  }


  // TODO: create adapters for other common Python types like Set, Dict, Tuple
  /*
    Represents a Python list. This provides a Chapel interface to Python lists,
    where the Python interpreter owns the list.
  */
  class PyList: Value {
    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
    }

    /*
      Get the size of the list. Equivalent to calling ``len(obj)`` in Python.

      :returns: The size of the list.
    */
    proc size: int throws {
      var size = PySequence_Size(this.get());
      this.check();
      return size;
    }

    /*
      Get an item from the list. Equivalent to calling ``obj[idx]`` in Python.

      :arg T: The Chapel type of the item to return.
      :arg idx: The index of the item to get.
      :returns: The item at the given index.
    */
    proc getItem(type T, idx: int): T throws {
      var item = PySequence_GetItem(this.get(), idx.safeCast(Py_ssize_t));
      this.check();
      return interpreter.fromPython(T, item);
    }
    /*
      Set an item in the list. Equivalent to calling ``obj[idx] = item`` in
      Python.

      :arg idx: The index of the item to set.
      :arg item: The item to set.
    */
    proc setItem(idx: int, item: ?) throws {
      PySequence_SetItem(this.get(),
                     idx.safeCast(Py_ssize_t),
                     interpreter.toPython(item));
      this.check();
    }
  }

  private proc checkFormatWithEltType(format: c_ptr(c_char),
                              itemSize: Py_ssize_t, type eltType): bool {
    if format == nil {
      // make sure item size matches numBytes
      // but since format is nil we cannot check the type more than this
      return numBytes(eltType) == itemSize;
    }
    if format.deref() == "c".toByte() && eltType == c_char then return true;
    if format.deref() == "b".toByte() && eltType == c_schar then return true;
    if format.deref() == "B".toByte() && eltType == c_uchar then return true;
    if format.deref() == "?".toByte() && eltType == bool then return true;
    if format.deref() == "h".toByte() && eltType == c_short then return true;
    if format.deref() == "H".toByte() && eltType == c_ushort then return true;
    if format.deref() == "i".toByte() && eltType == c_int then return true;
    if format.deref() == "I".toByte() && eltType == c_uint then return true;
    if format.deref() == "l".toByte() && eltType == c_long then return true;
    if format.deref() == "L".toByte() && eltType == c_ulong then return true;
    if format.deref() == "q".toByte() && eltType == c_longlong then return true;
    if format.deref() == "Q".toByte() && eltType == c_ulonglong then return true;
    if format.deref() == "n".toByte() && eltType == c_ssize_t then return true;
    if format.deref() == "N".toByte() && eltType == c_size_t then return true;
    if format.deref() == "f".toByte() && eltType == c_float then return true;
    if format.deref() == "d".toByte() && eltType == c_double then return true;
    return false;
  }

  /*
    Represents a Python array. This provides a Chapel interface to Python types
    that are array-like. This includes
    `array.array <https://docs.python.org/3/library/array.html>`_ and
    `numpy.ndarray <https://numpy.org/doc/stable/reference/generated/numpy.ndarray.html>`_.
  */
  class PyArray: Value {
    type eltType;
    @chpldoc.nodoc
    var view: Py_buffer;
    @chpldoc.nodoc
    var itemSize: Py_ssize_t;

    @chpldoc.nodoc
    proc init(type eltType) {
      this.eltType = eltType;
    }

    @chpldoc.nodoc
    proc init(type eltType, in interpreter: borrowed Interpreter,
              in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
      this.eltType = eltType;
    }
    @chpldoc.nodoc
    proc postinit() throws {
      if PyObject_CheckBuffer(this.get()) == 0 {
        throw new ChapelException("Object does not support buffer protocol");
      }
      var flags: c_int =
        PyBUF_SIMPLE | PyBUF_WRITABLE | PyBUF_FORMAT | PyBUF_ND;
      if PyObject_GetBuffer(this.get(), c_ptrTo(this.view), flags) == -1 {
        this.check();
        // this.check should have raised an exception, if it didn't, raise one
        throw new BufferError("Failed to get buffer");
      }

      if this.view.ndim > 1 {
        throw new ChapelException("Only 1D arrays are currently supported");
      }

      this.itemSize = PyBuffer_SizeFromFormat(this.view.format);
      if this.itemSize == -1 {
        if this.view.shape != nil {
          this.itemSize = this.view.itemsize;
        } else {
          // disregard itemsize, use 1
          this.itemSize = 1;
        }
      }

      if !checkFormatWithEltType(this.view.format, this.itemSize, this.eltType) {
        throw new ChapelException("Format does not match element type");
      }

    }

    proc deinit() {
      PyBuffer_Release(c_ptrTo(this.view));
    }

    proc array: [] {
      var buf = view.buf: c_ptr(this.eltType);
      var size = view.len / this.itemSize;
      var res = makeArrayFromPtr(buf, size);
      return res;
    }
  }

  @chpldoc.nodoc
  proc isSupportedArrayType(arr) param : bool {
    return isArrayType(arr.type) &&
           arr.rank == 1 &&
           arr.idxType == int &&
           arr.isDefaultRectangular();
  }

  /*
    Represents a handle to a Chapel array that is usable by Python code. This
    allows code to pass Chapel arrays to Python without copying the data. This
    only works for 1D local rectangular arrays.

    .. note::

       While Chapel arrays can be indexed arbitrarily by specifying a domain
       (e.g. ``var myArr: [2..by 4 #2]``), the equivalent Python array will
       also by indexed starting at 0 with a stride of 1. Methods like
       :proc:`~Array.getItem` do no translation of the domain and should be
       called with the Python interpretation of the index.


    To pass a Chapel array to a Python function, you would normally
    just use the Chapel array directly, resulting in the data being copied in.
    To avoid this copy, first create an :type:`Array` object, then pass that to
    the Python function.

    For example,

    .. code-block:: chapel

       myPythonFunction(NoneType, myChapelArray); // copies the data

       var arr = new Array(interpreter, myChapelArray);
       myPythonFunction(NoneType, arr); // no copy is done

    .. warning::

       If the array is invalidated or deallocated in Chapel, the Python code
       will crash when it tries to access the array.
  */
  class Array: Value {
    @chpldoc.nodoc
    type eltType = nothing;
    @chpldoc.nodoc
    var buffer: c_ptr(void);
    /*
      Create a new :type:`Array` object from a Chapel array.

      :arg interpreter: The interpreter that this object is associated with.
      :arg arr: The Chapel array to create the object from.
    */
    proc init(in interpreter: borrowed Interpreter, ref arr: []) throws
      where isSupportedArrayType(arr) {
      super.init(interpreter, ArrayTypes.createArray(arr), isOwned=true);
      this.eltType = arr.eltType;
    }
    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter, ref arr: []) throws
      where !isSupportedArrayType(arr) {
      compilerError("Only 1D local rectangular arrays are currently supported");
      this.eltType = nothing;
    }
    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              in obj: PyObjectPtr, isOwned: bool = true) {
      compilerError("Cannot create an Array from an existing PyObject");
      this.eltType = nothing;
    }

    /*
      Get the size of the array. Equivalent to calling ``len(obj)`` in Python or
      ``originalArray.size`` in Chapel.

      :returns: The size of the array.
    */
    proc size: int throws {
      var size = PySequence_Size(this.get());
      this.check();
      return size;
    }

    /*
      Get an item from the array. Equivalent to calling ``obj[idx]`` in Python
      or ``originalArray[idx]`` in Chapel.

      :arg idx: The index of the item to get.
      :returns: The item at the given index.
    */
    proc getItem(idx: int): eltType throws {
      var pyObj = PySequence_GetItem(this.get(), idx.safeCast(Py_ssize_t));
      this.check();
      return interpreter.fromPython(eltType, pyObj);
    }
    /*
      Set an item in the array. Equivalent to calling ``obj[idx] = item`` in
      Python or ``originalArray[idx] = item`` in Chapel.

      :arg idx: The index of the item to set.
      :arg item: The item to set.
    */
    proc setItem(idx: int, item: eltType) throws {
      var pyItem = interpreter.toPython(item);
      PySequence_SetItem(this.get(), idx.safeCast(Py_ssize_t), pyItem);
      this.check();
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
    Represents the current thread state. This saves and restores the current
    thread state.

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
      Saves the current thread state. If the state is already saved, this
      is a no-op.
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
    Casts a Chapel value to a Python object.
    This is the equivalent of calling ``obj.str()`` in Chapel code.
  */
  @chpldoc.nodoc
  operator:(v, type t: string): string throws
    where isSubtype(v.type, Value) || isSubtype(v.type, NoneType) {
    return v.str();
  }

  Value implements writeSerializable;
  Function implements writeSerializable;
  Module implements writeSerializable;
  Class implements writeSerializable;
  PyList implements writeSerializable;
  Array implements writeSerializable;
  NoneType implements writeSerializable;

  @chpldoc.nodoc
  override proc Value.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc Function.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc Module.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc Class.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc PyList.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc Array.serialize(writer, ref serializer) throws do
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
      extern proc mbstowcs(dest: c_ptr(c_wchar),
                           src: c_ptrConst(c_char),
                           n: c_size_t): c_size_t;
      var len: c_size_t = this.size.safeCast(c_size_t);
      var buf = allocate(c_wchar, len + 1, clear=true);
      mbstowcs(buf, this.c_str(), len);
      return buf;
    }
  }

  @chpldoc.nodoc
  module CPythonInterface {
    require "Python.h";
    require "marshal.h";
    require "PythonHelper/ChapelPythonHelper.h";
    private use CTypes;
    private use super.CWChar;
    import HaltWrappers;



    /*
      Types
    */
    extern type PyObject;
    type PyObjectPtr = c_ptr(PyObject);
    extern type PyTypeObject;
    type PyTypeObjectPtr = c_ptr(PyTypeObject);

    // technically this is an extern type defined by python, but if we treat it
    // as an opaque type, we can't use it in Chapel code in casts and such.
    // using c_ssize_t should be sufficient and safe
    type Py_ssize_t = c_ssize_t;

    extern type PyThreadState;
    type PyThreadStatePtr = c_ptr(PyThreadState);
    extern type PyGILState_STATE;

    /*
      PyConfig
    */
    extern record PyConfig {
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

    extern proc PyConfig_SetString(config_: c_ptr(PyConfig),
                                   config_str: c_ptr(c_ptr(c_wchar)),
                                   str: c_ptr(c_wchar)): PyStatus;
    extern proc PyConfig_SetBytesString(config_: c_ptr(PyConfig),
                                        config_str: c_ptr(c_ptr(c_wchar)),
                                        str: c_ptrConst(c_char)): PyStatus;

    extern type PyWideStringList;
    extern proc PyWideStringList_Append(list: c_ptr(PyWideStringList),
                                        str: c_ptr(c_wchar)): PyStatus;
    extern proc PyWideStringList_Insert(list: c_ptr(PyWideStringList),
                                        idx: c_int,
                                        str: c_ptr(c_wchar)): PyStatus;

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
    extern "chpl_Py_INCREF" proc Py_INCREF(obj: PyObjectPtr);
    extern "chpl_Py_DECREF" proc Py_DECREF(obj: PyObjectPtr);
    extern "chpl_Py_CLEAR" proc Py_CLEAR(obj: c_ptr(PyObjectPtr));
    extern proc PyMem_Free(ptr: c_ptr(void));
    extern proc PyObject_Str(obj: PyObjectPtr): PyObjectPtr; // `str(obj)`
    extern proc PyImport_ImportModule(name: c_ptrConst(c_char)): PyObjectPtr;

    extern "chpl_PY_VERSION_HEX" const PY_VERSION_HEX: uint(64);
    extern "chpl_PY_VERSION" const PY_VERSION: c_ptrConst(c_char);
    extern "chpl_PY_MAJOR_VERSION" const PY_MAJOR_VERSION: c_ulong;
    extern "chpl_PY_MINOR_VERSION" const PY_MINOR_VERSION: c_ulong;
    extern "chpl_PY_MICRO_VERSION" const PY_MICRO_VERSION: c_ulong;

    extern proc PySys_GetObject(name: c_ptrConst(c_char)): PyObjectPtr;


    /*
      Sub Interpreters
    */
    extern proc chpl_Py_NewIsolatedInterpreter(tstate: c_ptr(PyThreadStatePtr)): PyStatus;
    extern proc Py_EndInterpreter(tstate: PyThreadStatePtr);

    /*
      Global exec functions
    */
    extern proc PyRun_SimpleString(code: c_ptrConst(c_char));
    extern proc chpl_PyEval_GetFrameGlobals(): PyObjectPtr;


    extern var Py_eval_input: c_int;
    extern var Py_file_input: c_int;
    extern proc PyRun_String(code: c_ptrConst(c_char), start: c_int,
                             globals: PyObjectPtr,
                             locals: PyObjectPtr): PyObjectPtr;
    extern proc Py_CompileString(code: c_ptrConst(c_char),
                                 filename: c_ptrConst(c_char),
                                 start: c_int): PyObjectPtr;
    extern proc PyImport_ExecCodeModule(name: c_ptrConst(c_char),
                                        code: PyObjectPtr): PyObjectPtr;
    extern proc PyMarshal_ReadObjectFromString(data: c_ptrConst(c_char),
                                               size: Py_ssize_t): PyObjectPtr;


    /*
      Threading
    */
    extern proc PyEval_SaveThread(): PyThreadStatePtr;
    extern proc PyEval_RestoreThread(state: PyThreadStatePtr);
    extern proc PyThreadState_Get(): PyThreadStatePtr;
    extern proc PyThreadState_Swap(state: PyThreadStatePtr): PyThreadStatePtr;

    extern proc PyGILState_Ensure(): PyGILState_STATE;
    extern proc PyGILState_Release(state: PyGILState_STATE);

    /*
      Error handling
    */
    extern proc PyErr_Occurred(): PyObjectPtr;
    extern proc chpl_PyErr_GetRaisedException(): PyObjectPtr;
    extern proc PyErr_GivenExceptionMatches(given: PyObjectPtr,
                                            exc: PyObjectPtr): c_int;
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
    extern proc PyBytes_AsString(obj: PyObjectPtr): c_ptrConst(c_char);
    extern proc PyBytes_Size(obj: PyObjectPtr): Py_ssize_t;
    extern proc PyBytes_FromStringAndSize(s: c_ptrConst(c_char),
                                          size: Py_ssize_t): PyObjectPtr;

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
    extern proc PySequence_Size(obj: PyObjectPtr): Py_ssize_t;
    extern proc PySequence_GetItem(obj: PyObjectPtr,
                                   idx: Py_ssize_t): PyObjectPtr;
    extern proc PySequence_SetItem(obj: PyObjectPtr,
                                   idx: Py_ssize_t,
                                   value: PyObjectPtr);


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
    extern proc PyList_New(size: Py_ssize_t): PyObjectPtr;
    extern proc PyList_SetItem(list: PyObjectPtr,
                               idx: Py_ssize_t,
                               item: PyObjectPtr);
    extern proc PyList_GetItem(list: PyObjectPtr, idx: Py_ssize_t): PyObjectPtr;
    extern proc PyList_Size(list: PyObjectPtr): Py_ssize_t;
    extern proc PyList_Insert(list: PyObjectPtr,
                              idx: Py_ssize_t,
                              item: PyObjectPtr);
    extern proc PyList_Append(list: PyObjectPtr, item: PyObjectPtr);

    /*
      Sets
    */
    extern proc PySet_New(): PyObjectPtr;
    extern proc PySet_Add(set: PyObjectPtr, item: PyObjectPtr);
    extern proc PySet_Size(set: PyObjectPtr): Py_ssize_t;


    /*
      Dictionaries
    */
    extern proc PyDict_New(): PyObjectPtr;
    extern proc PyDict_SetItem(dict: PyObjectPtr,
                               key: PyObjectPtr,
                               value: PyObjectPtr);
    extern proc PyDict_SetItemString(dict: PyObjectPtr,
                                     key: c_ptrConst(c_char),
                                     value: PyObjectPtr);
    extern proc PyDict_GetItem(dict: PyObjectPtr,
                               key: PyObjectPtr): PyObjectPtr;
    extern proc PyDict_GetItemString(dict: PyObjectPtr,
                                     key: c_ptrConst(c_char)): PyObjectPtr;
    extern proc PyDict_Size(dict: PyObjectPtr): Py_ssize_t;
    extern proc PyDict_Keys(dict: PyObjectPtr): PyObjectPtr;

    extern proc PyObject_GetAttrString(obj: PyObjectPtr,
                                       name: c_ptrConst(c_char)): PyObjectPtr;
    extern proc PyObject_SetAttrString(obj: PyObjectPtr,
                                       name: c_ptrConst(c_char),
                                       value: PyObjectPtr);

    /*
      Tuples
    */
    extern proc PyTuple_Pack(size: Py_ssize_t, args...): PyObjectPtr;

    /*
      Functions
    */
    extern proc PyObject_Call(obj: PyObjectPtr,
                              args: PyObjectPtr,
                              kwargs: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallObject(obj: PyObjectPtr,
                                    args: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallNoArgs(obj: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallOneArg(obj: PyObjectPtr,
                                    arg: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallFunctionObjArgs(obj: PyObjectPtr,
                                             args...): PyObjectPtr;

    extern proc PyObject_CallMethodNoArgs(obj: PyObjectPtr,
                                          method: PyObjectPtr): PyObjectPtr;
    extern proc PyObject_CallMethodObjArgs(obj: PyObjectPtr,
                                           method: PyObjectPtr,
                                           args...): PyObjectPtr;


    /*
      Buffer protocol: https://docs.python.org/3/c-api/buffer.html
    */
    extern record Py_buffer {
      var buf: c_ptr(void);
      var obj: PyObjectPtr;
      var len: Py_ssize_t;
      var itemsize: Py_ssize_t;
      var readonly: c_int;
      var ndim: c_int;
      var format: c_ptr(c_char);
      var shape: c_ptr(Py_ssize_t);
      var strides: c_ptr(Py_ssize_t);
      var suboffsets: c_ptr(Py_ssize_t);
      var internal: c_ptr(void);
    }

    extern proc PyObject_CheckBuffer(obj: PyObjectPtr): c_int;
    extern proc PyObject_GetBuffer(obj: PyObjectPtr,
                                   view: c_ptr(Py_buffer),
                                   flags: c_int): c_int;
    extern proc PyBuffer_Release(view: c_ptr(Py_buffer));
    extern proc PyBuffer_SizeFromFormat(format: c_ptrConst(c_char)): Py_ssize_t;
    extern proc PyBuffer_IsContiguous(view: c_ptr(Py_buffer),
                                      order: c_char): c_int;
    extern proc PyBuffer_GetPointer(view: c_ptr(Py_buffer),
                                    inidices: c_ptr(Py_ssize_t)): c_ptr(void);
    extern proc PyBuffer_FromContiguous(view: c_ptr(Py_buffer),
                                        buf: c_ptr(void),
                                        len: Py_ssize_t,
                                        order: c_char): c_int;
    extern proc PyBuffer_ToContiguous(buf: c_ptr(void),
                                      view: c_ptr(Py_buffer),
                                      len: Py_ssize_t,
                                      order: c_char): c_int;
    extern proc PyObject_CopyData(dst: PyObjectPtr,
                                 src: PyObjectPtr): c_int;
    extern proc PyBuffer_FillContiguousStrides(ndims: Py_ssize_t,
                                              shape: c_ptr(Py_ssize_t),
                                              strides: c_ptr(Py_ssize_t),
                                              itemsize: c_int,
                                              order: c_char);
    extern proc PyBuffer_FillInfo(view: c_ptr(Py_buffer),
                                  exporter: PyObjectPtr,
                                  buf: c_ptr(void),
                                  len: Py_ssize_t,
                                  readonly: c_int,
                                  flags: c_int): c_int;


    extern "chpl_PyBUF_SIMPLE" const PyBUF_SIMPLE: c_int;
    extern "chpl_PyBUF_WRITABLE" const PyBUF_WRITABLE: c_int;
    extern "chpl_PyBUF_FORMAT" const PyBUF_FORMAT: c_int;
    extern "chpl_PyBUF_ND" const PyBUF_ND: c_int;

  }


  @chpldoc.nodoc
  module ArrayTypes {
    private use super.CPythonInterface;
    private use CTypes;
    require "PythonHelper/ArrayTypes.h";
    require "PythonHelper/ArrayTypes.c";

    extern proc createArrayTypes(): bool;
    extern proc registerArrayTypeEnum(): bool;

    proc typeToArraySuffix(type T) param {
      if isArrayType(T) then return "A";
      select T {
        when int(64) do return "I64";
        when uint(64) do return "U64";
        when int(32) do return "I32";
        when uint(32) do return "U32";
        when int(16) do return "I16";
        when uint(16) do return "U16";
        when int(8) do return "I8";
        when uint(8) do return "U8";
        when real(64) do return "R64";
        when real(32) do return "R32";
        when bool do return "Bool";
        otherwise do return "";
      }
    }

    proc createArray(ref arr: []): PyObjectPtr {
      type T = arr.eltType;
      param suffix = typeToArraySuffix(T);
      if suffix == "" then compilerError("Unsupported array type: " + T:string);

      param externalName = "createArray" + suffix;
      extern externalName
      proc createPyArray(arr: c_ptr(void),
                         size: Py_ssize_t, isOwned: bool): PyObjectPtr;

      if isArrayType(T) {
        var sub = allocate(PyObjectPtr, arr.size);
        for i in 0..#arr.size {
          sub(i) = createArray(arr(i));
        }
        return createPyArray(sub, arr.size.safeCast(Py_ssize_t), true);
      } else {
        return createPyArray(c_ptrTo(arr), arr.size.safeCast(Py_ssize_t), false);
      }

    }
  }
}
