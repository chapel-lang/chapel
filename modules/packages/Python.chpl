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


// TODO: represent python context managers as Chapel context managers
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
  and requires linking with the Python library. The ``embed-python.sh`` script
  provided with Chapel can be used to generate the necessary compiler flags.
  Assuming ``python3`` is installed into the system path, the following command
  can be used to compile Chapel code that uses this module:

  .. code-block:: bash

     chpl $($(chpl --print-chpl-home)/util/config/embed-python.sh) ...Chapel source files...

  Alternative Python installations can be used by setting the ``CHPL_PYTHON``
  environment variable:

  .. code-block:: bash

     chpl $(CHPL_PYTHON=/path/to/python3 $(chpl --print-chpl-home)/util/config/embed-python.sh) ...Chapel source files...

  .. warning::

    Chapel programs compiled in this way are compiled for a specific Python
    version. Attempting to run the compiled program with a different Python
    version may have unexpected results.

  Parallel Execution
  ------------------

  Running any Python code in parallel from Chapel will likely be no faster than
  serial execution due to the Global Interpreter Lock (GIL) in the Python
  interpreter. This module automatically handles the GIL in the public API.

  .. note::

     Newer Python versions offer a free-threading mode that allows multiple
     threads concurrently. This currently requires a custom build of Python. If
     you are using a Python interpreter like this, the GIL handling code in this
     module will become a no-op.

  The following demonstrates executing multiple Chapel tasks using a `coforall`
  and a single Python interpreter:

  .. literalinclude:: ../../../../test/library/packages/Python/doc-examples/CoforallTest.chpl
     :language: chapel
     :start-after: START_EXAMPLE
     :end-before: STOP_EXAMPLE

  The code works similarly with a data-parallel ``forall`` loop:

  .. literalinclude:: ../../../../test/library/packages/Python/doc-examples/ForallTest.chpl
     :language: chapel
     :start-after: START_EXAMPLE
     :end-before: STOP_EXAMPLE

  Although these examples use Chapel's task parallelism constructs,
  they will be no faster than running the tasks serially due to the GIL.

  Using Python Modules With Distributed Code
  -------------------------------------------

  Python has no built-in support for distributed memory, so each locale must
  create its own interpreter (and subsequent Python objects).

  The following example demonstrates how to create a Python interpreter and run
  a Python function on each locale:

  .. literalinclude:: ../../../../test/library/packages/Python/doc-examples/DistributedTest.chpl
     :language: chapel
     :start-after: START_EXAMPLE
     :end-before: STOP_EXAMPLE

  In this example, ``interp`` and ``func`` only exist for the body of the
  ``on`` block, Python objects can be made to persist beyond the scope of a
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

  .. literalinclude:: ../../../../test/library/packages/Python/doc-examples/Printing.chpl
     :language: chapel
     :start-after: START_EXAMPLE
     :end-before: STOP_EXAMPLE

  More Examples:
  --------------

  For more examples of using the Python module, see the test cases in
  `$CHPL_HOME/test/library/packages/Python/examples <https://github.com/chapel-lang/chapel/tree/main/test/library/packages/Python/examples>`_.

*/
@unstable("The Python module's interface is under active development and may change")
module Python {

  /*
  =======================
  Developer Documentation
  =======================

  This module makes heavy usage of the Python C API. Care should be taken when
  adding new calls to the C API, especially when it comes to reference counting
  and the GIL.

  Reference Counting
  ------------------

  Python objects are reference counted. When a Python object is created, it has
  a reference count of 1. When the reference count reaches 0 the object is
  deallocated. Normally, Python code that is interpreted has this handled by
  the interpreter. Using the C API, requires the programmer to manage the
  reference counts. This module hides this implementation detail from the user.

  There are two things to keep in mind.
    1. making sure that when Chapel is done with the object, the reference
       count is zero.
    2. making sure that while Chapel is still owning the object, the reference
       count is greater than zero.

  The `Value` base class has a field, `isOwned`, which determines if the
  reference count of the Python object will be decremented when the Chapel
  object is deleted. `toPython` creates new references and `fromPython` steals
  references (decrements the reference count). Most other functions don't touch
  the reference count. C API functions generally document if they return/take a
  new reference ("strong reference"), a borrowed reference (reference count is
  neither incremented or decremented), or a stolen reference (reference count
  is decremented). Make sure to check the behavior of new functions.

  Global Interpreter Lock (GIL)
  -----------------------------

  The Global Interpreter Lock (GIL) is a mutex that protects access to the
  interpreter state. Some newer builds of Python are "free-threaded" and don't
  have a GIL. Whether or not the GIL is present, this module will handle the
  GIL. The `PyGILState_Ensure` and `PyGILState_Release` functions are used to
  acquire and release the GIL; in free-threaded builds, these functions are
  no-ops.

  Python and non-python threads require special handling. The
  `PyGILState_[Ensure|Release]` functions not only acquire/release the GIL, but
  also automatically handle setting up new Python threads. From Chapel's
  perspective, `PyGILState_Ensure` must be called before any Python code and
  `PyGILState_Release` must be called afterwards. Failure to do so will result
  in undefined behavior, likely a segfault of some kind. You will frequently
  see the `var ctx = chpl_pythonContext.enter(); defer ctx.exit();` pattern
  in this module, which acquires the GIL and then releases it at the end of the
  current scope (or during stack unwinding due to an exception)

  Interacting with Chapel's threading model can also be problematic.
  `PyGILState_[Ensure|Release]` calls must be matched to each other on the same
  thread or deadlock will occur. NOTE: this can happen easily and
  transparently in Chapel, particularly when `on` blocks are used (they
  introduce new comm threads in multi-locale code). Additionally, the
  `Py_Initialize`/``Py_Finalize` family of calls must be made on the same
  thread. This module does additional work to facilitate that.

  As an overview, the interpreter is setup in the following way to properly
  handle the GIL.
  1. `new Interpreter()` spawns a new daemon thread which does the following:
     a. Initializes the Python interpreter b. acquires the gil and enters a
     state where it can invoke threads (i.e. `Py_BEGIN_ALLOW_THREADS`) c.
     signals that setup is done and goes to sleep, waiting for a signal to
     finish
  2. `Interpreter.deinit()` signals the daemon thread to finish and waits for
     it to finish a. the daemon thread releases the gil and thread state (i.e
     `Py_END_ALLOW_THREADS`) b. the daemon thread signals that it is done and
     exits

  All public API calls in this module should be thread safe, meaning they
  handle the GIL correctly. So for example, `new Module(interp, "mymod")`
  should properly acquire/release the GIL. But, internal functions like
  `importModuleInternal` do not (and should not) acquire/release the GIL. While
  matching pairs to `PyGILState_[Ensure|Release]` can be nested, there is no
  reason to and the less we do that, the less overhead. This convention is
  especially important for `[to|from]Python`. `[to|from]Python` is a part of
  the public API, and so must acquire/release the GIL. But these functions are
  so heavily used to implement other parts of the public API that the
  internal `[to|from]PythonInner` functions are used to avoid the overhead of
  acquiring/releasing the GIL. Essentially, `[to|from]Python` acquires the GIL,
  calls `[to|from]PythonInner`, and then releases the GIL.

  Important documentation to read up on Python threading and the GIL
    - https://docs.python.org/3/c-api/init.html#thread-state-and-the-global-interpreter-lock
    - https://docs.python.org/3/c-api/init.html#c.Py_Initialize
    - https://docs.python.org/3/c-api/init.html#c.Py_FinalizeEx
  */


  private use CTypes;
  private import List;
  private import Map;
  private import Set;

  private use CPythonInterface except PyObject;
  private import this.ArrayTypes;
  private use CWChar;
  private use OS.POSIX only getenv;
  private import this.PThread;

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

  /*
    Internal object to handle the GIL. This is used to
    ensure that the GIL is properly acquired and released. The common API of
    `enter`/`exit` acquires the GIL.

    Proper usage of this object looks like this:

    .. code-block:: chapel

       var ctx = chpl_pythonContext.enter();
       // some python operation
       ctx.exit();

    This is commonly written on entry to functions that call Python code as:

    .. code-block:: chapel

       proc someFunction() {
         var ctx = chpl_pythonContext.enter();
         defer ctx.exit();
         // some python operation
       }
  */
  @chpldoc.nodoc
  record chpl_pythonContext {
    var gil: PyGILState_STATE;
    inline proc init(in gil: PyGILState_STATE) {
      this.gil = gil;
    }
    inline proc deinit() {}

    inline proc type enter() {
      var gil = PyGILState_Ensure();
      return new chpl_pythonContext(gil);
    }
    inline proc exit() {
      PyGILState_Release(this.gil);
    }
  }


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

    var sep = interp.fromPythonInner(string, sepPy);
    return sep;
  }

  /*
    Helper function to convert object pointer to string
    for developer debugging purposes.
  */
  private proc toStringDev(obj: PyObjectPtr) {
    var pyStr = PyObject_Str(obj);
    defer Py_DECREF(pyStr);
    if pyStr == nil {
      halt("Failed to convert PyObject to a Python string");
    }
    var str = PyUnicode_AsUTF8(pyStr);
    if str == nil {
      halt("Failed to convert Python string to a C string");
    }
    return try! string.createCopyingBuffer(str);
  }

  private proc throwChapelException(message: string) throws {
    throw new ChapelException(message);
  }

  @chpldoc.nodoc
  record signalPair {
    var setupDone: c_ptr(PThread.pthreadSignal);
    var done: c_ptr(PThread.pthreadSignal);
  }
  @chpldoc.nodoc
  var interpreterThreadError: owned Error? = nil;
  private proc interpreterThread(arg: c_ptr(void)): c_ptr(void) {

    var signals = (arg:c_ptr(signalPair)).deref();

    try {
      // preinit
      var preconfig: PyPreConfig;
      PyPreConfig_InitIsolatedConfig(c_ptrTo(preconfig));
      preconfig.utf8_mode = 1;
      checkPyStatus(Py_PreInitialize(c_ptrTo(preconfig)));

      // init
      var config_: PyConfig;
      var cfgPtr = c_ptrTo(config_);
      // we want an isolated config for things like LC_ALL, but we still want
      // to use things like PYTHONPATH
      PyConfig_InitIsolatedConfig(cfgPtr);
      config_.isolated = 0;
      config_.use_environment = 1;
      config_.user_site_directory = 1;
      config_.site_import = 1;
      defer PyConfig_Clear(cfgPtr);

      // check VIRTUAL_ENV, if it is set, make it the executable
      var venv = getenv("VIRTUAL_ENV".c_str());
      if venv != nil {
        use OS.POSIX only memcpy;
        extern const CHPL_RT_MD_STR_COPY_DATA: chpl_mem_descInt_t;
        extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;

        // Explicitly combine the memory to avoid an issue with gasnet
        var venvLen = strLen(venv);
        var pythonPathExt = "/bin/python";
        var newSize = venvLen + pythonPathExt.size;
        var execCStr = chpl_here_alloc(newSize+1,
                                       CHPL_RT_MD_STR_COPY_DATA -
                                       chpl_memhook_md_num()): venv.type;
        memcpy(execCStr, venv, venvLen.safeCast(c_size_t));
        memcpy(execCStr+venvLen, pythonPathExt.buff,
               pythonPathExt.size.safeCast(c_size_t));
        execCStr[newSize] = 0;
        defer chpl_here_free(execCStr);

        const executable = string.createBorrowingBuffer(execCStr);
        const wideExecutable = executable.c_wstr();
        defer deallocate(wideExecutable);
        // ideally this just sets config_.home
        // but by setting executable we can reuse the python logic to
        // determine the locale (in the string sense, not the chapel sense)
        checkPyStatus(
          PyConfig_SetString(
            cfgPtr, c_ptrTo(config_.executable), wideExecutable));
      }

      // initialize
      checkPyStatus(Py_InitializeFromConfig(cfgPtr));
    } catch e {
      // if any errors occur, signal that setup is done and exit
      interpreterThreadError = e;
      signals.setupDone.deref().signal();
      return nil;
    }

    //
    // grab the GIL and enter a state where we can invoke threads
    //
    var gil = PyGILState_Ensure();
    var ts = PyEval_SaveThread();

    //
    // setup is done
    //
    signals.setupDone.deref().signal();

    //
    // wait for deinit to be called
    //
    signals.done.deref().wait();

    //
    // restore the thread state so we can release the gil and exit
    //
    PyEval_RestoreThread(ts);
    PyGILState_Release(gil);
    Py_Finalize();
    return nil;

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
    var operatorModule: PyObjectPtr = nil;

    @chpldoc.nodoc
    var anonModuleCounter: atomic int = 0;

    // the done signal is a field so it has the same lifetime as the
    // helper thread
    @chpldoc.nodoc
    var done: PThread.pthreadSignal;
    @chpldoc.nodoc
    var thread: PThread.pthread_t;

    @chpldoc.nodoc
    proc init(isSubInterpreter: bool = false) {
      this.isSubInterpreter = isSubInterpreter;
      init this;
    }
    @chpldoc.nodoc
    proc postinit() throws {
      if this.isSubInterpreter then return;

      var setupDone = new PThread.pthreadSignal();
      var signals = new signalPair(c_ptrTo(setupDone), c_ptrTo(this.done));

      //
      // Do all Python initialization/finalization in a single task.
      // Initialization will be done, then this thread will block on 'done'.
      // When the deinit is called, it will unblock this thread and finalize
      // NOTE: this is done with a manual pthread to work around
      // https://github.com/chapel-lang/chapel/issues/26855
      //
      PThread.pthread_create(c_ptrTo(this.thread),
                             nil,
                             c_ptrTo(interpreterThread),
                             c_ptrTo(signals));

      //
      // blocks until setup is done, once setup is done we can return
      //
      setupDone.wait();

      // check for an error from the pthread
      if interpreterThreadError != nil {
        throw interpreterThreadError;
      }

      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

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

      this.operatorModule = this.importModuleInternal("operator");
      if this.operatorModule == nil {
        throwChapelException("Failed to import operator module");
      }

      if pyMemLeaks {
        // import objgraph
        this.objgraph = this.importModuleInternal("objgraph");
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

      var ctx = chpl_pythonContext.enter();

      Py_DECREF(this.operatorModule);

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


      ctx.exit();

      //
      // we are done, the thread keeping the interpreter alive can finish
      //
      this.done.signal();
      PThread.pthread_join(this.thread, nil);
    }

    /*
      Import a Python module. This is equivalent to calling ``import modName``
      in Python.

      .. warning::

         Importing a second module with the same name will overwrite the first.

      :arg modName: The name of the module that will be created. Note that if
                    the module name already exists, it will be replaced.
    */
    proc importModule(modName: string): owned Module throws {
      return new Module(this, modName);
    }

    /*
      Create a Python module, using the provided ``moduleContents``. This is
      equivalent to putting the code in a file, and then importing the file
      via the file/module name.

      This function will generate a unique module name. If a specific module
      name is desired, use the :proc:`createModule` 2 argument overload, which
      takes ``modName`` and ``moduleContents``.

      :arg moduleContents: The contents of the module. This can be a
                           :type:`~String.string` of Python code or a
                           :type:`~Bytes.bytes` object containing Python
                           bytecode (i.e. from a ``*.pyc`` file).
    */
    proc createModule(moduleContents): owned Module throws
      where moduleContents.type == string || moduleContents.type == bytes {
      return this.createModule(
        "chpl_anon_module_" + this.anonModuleCounter.fetchAdd(1):string,
        moduleContents);
    }
    /*
      Create a Python module, using the provided ``moduleContents``. This is
      equivalent to putting the code in a file, and then importing the file
      via the file/module name.

      .. warning::

         Importing a second module with the same name will overwrite the first.

      :arg modName: The name of the module that will be created. Note that if
                    the module name already exists, it will be replaced.
      :arg moduleContents: The contents of the module. This can be a
                           :type:`~String.string` of Python code or a
                           :type:`~Bytes.bytes` object containing Python
                           bytecode (i.e. from a ``*.pyc`` file).
    */
    proc createModule(modName: string, moduleContents): owned Module throws
      where moduleContents.type == string || moduleContents.type == bytes {
      return new Module(this, modName, moduleContents);
    }

    /*
      Load a `Python Pickle <https://docs.python.org/3/library/pickle.html>`_
      as a Python object.

      :arg pickle: The pickle data to load
    */
    proc load(pickle: bytes): owned Value throws {
      return new Value(this, pickle);
    }

    /*
      Compile a lambda function like 'lambda x,: x + 1'

      Note: this only works with lambdas that accept a tuple of arguments,
      like 'x,' or '\*args'

      For example:
      .. code-block:: chapel

         interpreter.compileLambda("lambda x, y,: x + y");
    */
    proc compileLambda(code: string): owned Function throws {
      return new Function(this, code);
    }

    /*
      Get a Python object by name. This will either get a Python global
      variable, (like `globals()[name]` in Python) or a Python builtin by name.

      This will first query the current globals, and if the object is not found,
      it will query the builtins.

      :arg t: The Chapel type of the value to return.
      :arg attr: The name of the attribute/field to access.
    */
    pragma "docs only"
    proc get(type t=owned Value, attr: string): t throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc get(type t, attr: string): t throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var __main__ = PyImport_AddModule("__main__");
      this.checkException();
      if PyObject_HasAttrString(__main__, attr.c_str()) == 1 {
        var pyObj = PyObject_GetAttrString(__main__, attr.c_str());
        this.checkException();
        return this.fromPythonInner(t, pyObj);
      }

      var builtins = chpl_PyEval_GetFrameBuiltins();
      if builtins != nil {
        var pyObj = PyDict_GetItemString(builtins, attr.c_str());
        this.checkException();
        if pyObj != nil {
          Py_INCREF(pyObj); // PyDict_GetItemString is borrowed
          return this.fromPythonInner(t, pyObj);
        }
      }

      throw new ChapelException("Attribute not found: " + attr);
    }
    @chpldoc.nodoc
    proc get(attr: string): owned Value throws do
      return this.get(owned Value, attr);

    /*
      Sets a global variable. Equivalent to the following in Python:

      .. code-block:: python

         global attr
         attr = value

      :arg attr: The name of the global variable.
      :arg value: The value of new global variable.
    */
    proc set(attr: string, value: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var __main__ = PyImport_AddModule("__main__");
      this.checkException();
      var globals = PyModule_GetDict(__main__);
      this.checkException();

      var pyValue = this.toPythonInner(value);
      defer Py_DECREF(pyValue); // PyDict_SetItem doesn't steal a reference
      PyDict_SetItemString(globals, attr.c_str(), pyValue);
      this.checkException();
    }

    /*
      Remove a global variable. Equivalent to the following in Python:

      .. code-block:: python

         global attr
         del attr

      :arg attr: The name of the global variable to remove.
    */
    proc del(attr: string) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var __main__ = PyImport_AddModule("__main__");
      this.checkException();
      var globals = PyModule_GetDict(__main__);
      this.checkException();

      PyDict_DelItemString(globals, attr.c_str());
      this.checkException();
    }

    /*
      Execute a snippet of Python code within the context of the current
      interpreter. This function has access to all global
      variables in the interpreter, and can be pass additional extra variables
      using the ``kwargs`` argument.
    */
    proc run(code: string, kwargs: ? = none) throws
    where kwargs.type == nothing || kwargs.isAssociative() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var __main__ = PyImport_AddModule("__main__");
      this.checkException();
      var globals = PyModule_GetDict(__main__);
      this.checkException();

      var locals;
      if kwargs.type != nothing then
        locals = this.toPythonInner(kwargs);
      else
        locals = nil:PyObjectPtr;

      PyRun_String(code.c_str(), Py_file_input, globals, locals);
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
      like 'x,' or '\*args'

      Creates a new reference
    */
    @chpldoc.nodoc
    proc compileLambdaInternal(l: string): PyObjectPtr throws {
      var __main__ = PyImport_AddModule("__main__");
      this.checkException();
      var globals = PyModule_GetDict(__main__);
      this.checkException();

      // this is the equivalent of `eval(compile(l, '<string>', 'eval'))`
      // we can also do `Py_CompileString` -> `PyFunction_New`?
      var code = PyRun_String(l.c_str(), Py_eval_input, globals, nil);
      this.checkException();
      return code;
    }

    /*
      Compile a chunk of Python code as a module and return the module object.

      Creates a new reference
    */
    @chpldoc.nodoc
    proc compileModule(s: string, modname: string): PyObjectPtr throws
    {
      var code = chpl_Py_CompileString(s.c_str(), "<string>", Py_file_input);
      this.checkException();
      defer Py_DECREF(code);

      var mod = PyImport_ExecCodeModule(modname.c_str(), code);
      this.checkException();
      return mod;
    }

    /*
      Interpret a chunk of Python bytecode as a Python module and return the
      module object.

      Creates a new reference
    */
    @chpldoc.nodoc
    proc compileModule(b: bytes, modname: string): PyObjectPtr throws {
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
      var pickle = importModuleInternal("pickle");
      this.checkException();
      defer Py_DECREF(pickle);
      var loads = PyObject_GetAttrString(pickle, "loads");
      this.checkException();
      defer Py_DECREF(loads);
      var pyBytes = toPythonInner(b);
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
      var importlib_util = importModuleInternal("importlib.util");
      this.checkException();
      defer Py_DECREF(importlib_util);
      var MAGIC_NUMBER_py =
        PyObject_GetAttrString(importlib_util, "MAGIC_NUMBER");
      this.checkException();
      var MAGIC_NUMBER = this.fromPythonInner(bytes, MAGIC_NUMBER_py);
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

      This method requires the GIL to be held, calling it without doing so will
      result in undefined behavior. See :type:`GIL`.

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
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var stdout = PySys_GetObject("stdout");
      if stdout == nil then throw new ChapelException("stdout not found");

      var flushStr = this.toPythonInner("flush");
      defer Py_DECREF(flushStr);

      PyObject_CallMethodNoArgs(stdout, flushStr);
      if flushStderr {
        var stderr = PySys_GetObject("stderr");
        if stderr == nil then throw new ChapelException("stderr not found");
        PyObject_CallMethodNoArgs(stderr, flushStr);
      }
    }

    @chpldoc.nodoc
    inline proc importModuleInternal(in modName: string): PyObjectPtr throws {
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
    proc toPython(const val): PyObjectPtr throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();
      return toPythonInner(val);
    }
    @chpldoc.nodoc
    proc toPythonInner(const val: ?t): PyObjectPtr throws {
      for converter in this.converters {
        if converter.handlesType(t) {
          // make sure to handle the gil for users
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
      } else if isTupleType(t) {
        return toTuple(val);
      } else if isSubtype(t, List.list) {
        return toList(val);
      } else if isSubtype(t, Set.set) {
        return toSet(val);
      } else if isSubtype(t, Map.map) {
        return toDict(val);
      } else if isSubtype(t, Value?) {
        var borrowedVal = val: borrowed Value;
        Py_INCREF(borrowedVal.getPyObject());
        return borrowedVal.getPyObject();
      } else if t == NoneType {
        return Py_None;
      } else {
        throw new ChapelException(
          "Unsupported toPython type: '" + t:string + "'"
        );
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
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();
      return fromPythonInner(t, obj);
    }
    @chpldoc.nodoc
    proc fromPythonInner(type t, obj: PyObjectPtr): t throws {
      if isClassType(t) &&
         (!isSharedClassType(t) &&
          !isOwnedClassType(t) &&
          !isUnmanagedClassType(t)) {
        compilerError("fromPython only supports shared, owned, and unmanaged classes");
      }
      if isSubtype(t, Array?) {
        compilerError("Cannot create an Array from an existing PyObject");
      }

      for converter in this.converters {
        if converter.handlesType(t) {
          // make sure to handle the gil for users
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
          throw new ChapelException(
            "Unsupported fromPython array type: '" + t:string + "'"
          );
        }
      } else if isTupleType(t) {
        return fromTuple(t, obj);
      } else if isSubtype(t, List.list) {
        return fromList(t, obj);
      } else if isSubtype(t, Set.set) {
        return fromSet(t, obj);
      } else if isSubtype(t, Map.map) {
        return fromDict(t, obj);
      } else if isSubtype(t, Function?) {
        return new t(this, "<unknown>", obj);
      } else if isSubtype(t, Value?) {
        return new t(this, obj);
      } else if isSubtype(t, PyObjectPtr) {
        return obj;
      } else if t == NoneType {
        // returning NoneType can be used to ignore a return value
        // but if its not actually None, we still need to decrement the reference count
        if obj != Py_None then Py_DECREF(obj);
        return None;
      } else {
        throw new ChapelException(
          "Unsupported fromPython type: '" + t:string + "'"
        );
      }
    }

    /*
      Convert a tuple to a python tuple.  Returns a new reference
    */
    @chpldoc.nodoc
    proc toTuple(tup): PyObjectPtr throws
      where isTupleType(tup.type) {

      var pyTup = PyTuple_New(tup.size.safeCast(Py_ssize_t));
      this.checkException();
      for param idx in 0..<tup.size {
        var pyValue = toPythonInner(tup(idx));
        PyTuple_SetItem(pyTup, idx.safeCast(Py_ssize_t), pyValue);
        this.checkException();
      }
      return pyTup;
    }

    /*
      Convert a python tuple to a tuple.  Steals a reference to obj.
    */
    @chpldoc.nodoc
    proc fromTuple(type T, obj: PyObjectPtr): T throws
      where isTupleType(T) {

      var res: T;
      var objSize = PyTuple_Size(obj);
      this.checkException();
      if (res.size < objSize) {
        throw new ChapelException("type specified was smaller than returned value");
      } else if (res.size > objSize) {
        throw new ChapelException("type specified was larger than returned value");
      }

      for param i in 0..<res.size {
        var elm = PyTuple_GetItem(obj, i.safeCast(Py_ssize_t));
        this.checkException();
        Py_INCREF(elm);
        res(i) = fromPythonInner((res(i)).type, elm);
        this.checkException();
      }
      Py_DECREF(obj);
      return res;
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
        PyList_SetItem(pyList, i.safeCast(Py_ssize_t), toPythonInner(arr[idx]));
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
        PyList_SetItem(pyList, i.safeCast(Py_ssize_t), toPythonInner(l(i)));
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

      // if it's a sequence with a known size, we can just iterate over it
      var res: T;
      if PySequence_Size(obj) != res.size.safeCast(Py_ssize_t) {
        throw new ChapelException("Size mismatch");
      }
      for i in 0..<res.size {
        const idx = res.domain.orderToIndex(i);
        var elm = PySequence_GetItem(obj, i.safeCast(Py_ssize_t));
        this.checkException();
        res[idx] = fromPythonInner(res.eltType, elm);
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
          res.pushBack(fromPythonInner(T.eltType, item));
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
          res.pushBack(fromPythonInner(T.eltType, item));
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
    /* Not currently used, so commented out.  I think this is what we would
       write if we did want it.
    @chpldoc.nodoc
    proc toSet(arr): PyObjectPtr throws
      where isArrayType(arr.type) &&
            arr.rank == 1 && arr.isDefaultRectangular() {
      var pySet = PySet_New(nil);
      this.checkException();
      for a in arr {
        PySet_Add(pySet, toPython(a));
        this.checkException();
      }
      return pySet;
    }
    */

    /*
      Convert a chapel set to a python set. Returns a new reference
    */
    @chpldoc.nodoc
    proc toSet(s: Set.set(?)): PyObjectPtr throws {
      var pySet = PySet_New(nil);
      this.checkException();
      for i in s {
        PySet_Add(pySet, toPythonInner(i));
        this.checkException();
      }
      return pySet;
    }

    /*
      Converts a python set to an array
    */
    /* Not currently used, so commented out.  I think this is what we would
       write if we did want it.
    @chpldoc.nodoc
    proc fromSet(type T, obj: PyObjectPtr): T throws
      where isArrayType(T) {

      if (PySet_Check(obj) == 0) then
        throw new ChapelException("Can only convert a set to an array");

      // if it's a sequence with a known size, we can just iterate over it
      var res: T;
      if PySet_Size(obj) != res.size.safeCast(Py_ssize_t) {
        throw new ChapelException("Size mismatch");
      }
      var it = PyObject_GetIter(obj);
      this.checkException();
      for i in 0..<res.size {
        const idx = res.domain.orderToIndex(i);
        var elm = PyIter_Next(it);
        this.checkException();
        res[idx] = fromPython(res.eltType, elm);
        this.checkException();
      }
      Py_DECREF(obj);
      return res;
    }
    */

    /*
      Convert a python set to a Chapel set. Steals a reference to obj.
    */
    @chpldoc.nodoc
    proc fromSet(type T, obj: PyObjectPtr): T throws where isSubtype(T, Set.set) {
      var it = PyObject_GetIter(obj);
      this.checkException();
      var res = new T();
      for 0..<PySequence_Size(obj) {
        var elem = PyIter_Next(it);
        this.checkException();
        res.add(fromPythonInner(T.eltType, elem));
      }
      Py_DECREF(obj);
      return res;
    }


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
        var pyKey = toPythonInner(key);
        var pyValue = toPythonInner(arr[key]);
        PyDict_SetItem(pyDict, pyKey, pyValue);
        this.checkException();
      }
      return pyDict;
    }

    /*
      Convert a chapel map to a python dict.  Returns a new reference
    */
    @chpldoc.nodoc
    proc toDict(m: Map.map(?)): PyObjectPtr throws {
      var pyDict = PyDict_New();
      this.checkException();
      for key in m.keys() {
        var pyKey = toPythonInner(key);
        var pyValue = toPythonInner(m[key]);
        PyDict_SetItem(pyDict, pyKey, pyValue);
        Py_DECREF(pyValue);
        this.checkException();
      }
      return pyDict;
    }

    /*
      Converts a python dictionary to an associative array.
      Steals a reference to obj.
    */
    @chpldoc.nodoc
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

        var keyVal = this.fromPythonInner(keyType, key);
        dom.add(keyVal);
        arr[keyVal] = this.fromPythonInner(valType, val);
      }

      Py_DECREF(obj);
      return arr;
    }

    /*
      Converts a python dictionary to a chapel map.  Steals a reference to obj.
    */
    @chpldoc.nodoc
    proc fromDict(type T, obj: PyObjectPtr): T throws
      where isSubtype(T, Map.map) {
      var m = new T();
      type keyType = m.keyType;
      type valType = m.valType;

      var keys = PyDict_Keys(obj);
      defer Py_DECREF(keys);
      this.checkException();
      for i in 0..<PyList_Size(keys) {
        var key = PyList_GetItem(keys, i);
        this.checkException();
        var val = PyDict_GetItem(obj, key);
        this.checkException();

        Py_INCREF(key);
        var keyVal = this.fromPythonInner(keyType, key);
        Py_INCREF(val);
        m.add(keyVal, this.fromPythonInner(valType, val));
      }

      Py_DECREF(obj);
      return m;
    }

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
      var str = interp.fromPythonInner(string, py_str);
      Py_DECREF(exc);
      if PyErr_GivenExceptionMatches(exc, PyExc_ImportError) != 0 {
        return new ImportError(str);
      } else if PyErr_GivenExceptionMatches(exc, PyExc_KeyError) != 0 {
        return new KeyError(str);
      } else if PyErr_GivenExceptionMatches(exc, PyExc_BufferError) != 0 {
        return new BufferError(str);
      } else if PyErr_GivenExceptionMatches(exc, PyExc_NotImplementedError) != 0 {
        return new NotImplementedError(str);
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
    Represents a NotImplementedError in the Python code
  */
  class NotImplementedError: PythonException {
    proc init(in message: string) {
      super.init(message);
    }
  }

  /*
    Represents a KeyError in the Python code
  */
  class KeyError: PythonException {
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
    Represents a Python value, it handles reference counting and is owned by
    default.
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

    // Developer note: all initializers should acquire the
    // gil into gilState, then the postinit should release it. subclasses can
    // and should assume the gil is held
    // this is because we can only check for exceptions with the gil held, but
    // if we acquire the gil it will clear the exception from the last call
    // ideally, this extra field is not needed and the exception checking occurs
    // in the `init` itself, but throwing inits interact poorly with inheritance
    @chpldoc.nodoc
    var ctxInitState: chpl_pythonContext;

    /*
      Takes ownership of an existing Python object, pointed to by ``obj``

      :arg interpreter: The interpreter that this object is associated with.
      :arg obj: The :type:`~CTypes.c_ptr` to the existing object.
      :arg isOwned: Whether this object owns the Python object.
                    This is true by default.
    */
    proc init(in interpreter: borrowed Interpreter,
              obj: PyObjectPtr,
              isOwned: bool = true) {
      this.interpreter = interpreter;
      this.obj = obj;
      this.isOwned = isOwned;
      this.ctxInitState = chpl_pythonContext.enter();
      init this;
    }

    /*
      Creates a new Python object from a pickle.
      See :proc:`Interpreter.load`.

      :arg interpreter: The interpreter that this object is associated with.
      :arg pickleData: The pickle data to load.
    */
    proc init(in interpreter: borrowed Interpreter, pickleData: bytes) throws {
      this.interpreter = interpreter;
      this.isOwned = true;
      this.ctxInitState = chpl_pythonContext.enter();
      init this;
      // Only catch-less try! statements are allowed in initializers for now :(
      proc helper() throws {
        try {
          this.obj = this.interpreter.loadPickle(pickleData);
        } catch e {
          // an exception thrown from the init will not result in a call to the postinit
          // or the deinit, so we have to handle that stuff here
          defer this.ctxInitState.exit();
          // rethrow the exception
          throw e;
        }
      }
      helper();
    }
    /*
      Creates a new Python object from a Chapel value.

      :arg interpreter: The interpreter that this object is associated with.
      :arg value: The Chapel value to convert to a Python object.
    */
    proc init(in interpreter: borrowed Interpreter, value: ?) throws {
      this.interpreter = interpreter;
      this.isOwned = true;
      this.ctxInitState = chpl_pythonContext.enter();
      init this;
      // Only catch-less try! statements are allowed in initializers for now :(
      proc helper() throws {
        try {
          this.obj = this.interpreter.toPythonInner(value);
        } catch e {
          // an exception thrown from the init will not result in a call to the postinit
          // or the deinit, so we have to handle that stuff here
          defer this.ctxInitState.exit();
          // rethrow the exception
          throw e;
        }
      }
      helper();
    }

    @chpldoc.nodoc
    proc postinit() throws {
      defer this.ctxInitState.exit();
      this.interpreter.checkException();
    }

    @chpldoc.nodoc
    proc deinit() {
      if this.isOwned {
        // we only need the gil, not the whole context
        var ctx = chpl_pythonContext.enter();
        defer ctx.exit();
        Py_DECREF(this.obj);
      }
    }

    // TODO: this may crash if the gil is not held, should we continue to expose
    // this to users?
    /*
      Check if an exception has occurred in the interpreter.

      ``val.check();`` is equivalent to ``val.interpreter.checkException();``

      See :proc:`~Interpreter.checkException` for more information.
    */
    inline proc check() throws do this.interpreter.checkException();

    /*
      Returns the string representation of the object.
      This is the same as casting to a string.

      Equivalent to calling ``str(obj)`` in Python.
    */
    proc str(): string throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();
      var pyStr = PyObject_Str(this.obj);
      this.interpreter.checkException();
      var res = interpreter.fromPythonInner(string, pyStr);
      return res;
    }

    /*
      Returns the debug string representation of the object.

      Equivalent to calling ``repr(obj)`` in Python.
    */
    proc repr(): string throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();
      var pyStr = PyObject_Repr(this.obj);
      this.interpreter.checkException();
      var res = interpreter.fromPythonInner(string, pyStr);
      return res;
    }

    /*
      Returns the type of the object.

      Equivalent to calling ``type(obj)`` in Python.
    */
    proc pyType(): owned Value throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();
      var pyType = PyObject_Type(this.obj);
      this.interpreter.checkException();
      var res = interpreter.fromPythonInner(owned Value, pyType);
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
    pragma "docs only"
    proc this(type retType = owned Value,
              const args...,
              kwargs:?=none): retType throws
              where kwargs.isAssociative() do compilerError("docs only");

    pragma "last resort"
    @chpldoc.nodoc
    proc this(type retType,
              const args...,
              kwargs:?=none): retType throws
              where kwargs.isAssociative() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArg = this.packTuple((...args));
      defer Py_DECREF(pyArg);
      return callInternal(retType, this.getPyObject(), pyArg, kwargs);
    }
    pragma "last resort"
    @chpldoc.nodoc
    proc this(const args...,
              kwargs:?=none): owned Value throws
              where kwargs.isAssociative() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArg = this.packTuple((...args));
      defer Py_DECREF(pyArg);
      return callInternal(owned Value, this.getPyObject(), pyArg, kwargs);
    }
    pragma "last resort"
    @chpldoc.nodoc
    proc this(type retType,
              kwargs:?=none): retType throws where kwargs.isAssociative() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArgs = Py_BuildValue("()");
      defer Py_DECREF(pyArgs);
      return callInternal(retType, this.getPyObject(), pyArgs, kwargs);
    }
    pragma "last resort"
    @chpldoc.nodoc
    proc this(kwargs:?=none): owned Value throws where kwargs.isAssociative() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArgs = Py_BuildValue("()");
      defer Py_DECREF(pyArgs);
      return callInternal(owned Value, this.getPyObject(), pyArgs, kwargs);
    }

    @chpldoc.nodoc
    proc this(type retType, const args...): retType throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArg = this.packTuple((...args));
      defer Py_DECREF(pyArg);
      return callInternal(retType, this.getPyObject(), pyArg, none);
    }
    @chpldoc.nodoc
    proc this(const args...): owned Value throws do
      return this(owned Value, (...args));
    @chpldoc.nodoc
    proc this(type retType = owned Value): retType throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyRes = PyObject_CallNoArgs(this.getPyObject());
      this.interpreter.checkException();

      var res = interpreter.fromPythonInner(retType, pyRes);
      return res;
    }

    @chpldoc.nodoc
    proc packTuple(const args...) throws {
      var pyArgs: args.size * PyObjectPtr;
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPythonInner(args(i));
      }
      defer for pyArg in pyArgs do Py_DECREF(pyArg);

      var pyArg = PyTuple_Pack(args.size.safeCast(Py_ssize_t), (...pyArgs));
      interpreter.checkException();

      return pyArg;
    }
    @chpldoc.nodoc
    proc callInternal(type retType,
                      pyFunc: PyObjectPtr,
                      pyArg: PyObjectPtr,
                      kwargs: ?t): retType throws {
      var pyKwargs;
      if t != nothing then pyKwargs = interpreter.toPythonInner(kwargs);
                      else pyKwargs = nil;

      var pyRes = PyObject_Call(pyFunc, pyArg, pyKwargs);
      this.interpreter.checkException();

      var res = interpreter.fromPythonInner(retType, pyRes);
      return res;
    }

    /*
      Access an attribute/field of this Python object. This is equivalent to
      calling ``getattr(obj, attr)`` or ``obj[attr]`` in Python.

      This method can be used as a general accessor for Python objects.
      For example:

      .. literalinclude:: ../../../../test/library/packages/Python/doc-examples/GetFac.chpl
         :language: chapel
         :start-after: START_EXAMPLE
         :end-before: STOP_EXAMPLE

      :arg t: The Chapel type of the value to return.
      :arg attr: The name of the attribute/field to access.
    */
    pragma "docs only"
    proc get(type t=owned Value, attr: string): t throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc get(type t, attr: string): t throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyAttr = PyObject_GetAttrString(this.getPyObject(), attr.c_str());
      interpreter.checkException();

      var res = interpreter.fromPythonInner(t, pyAttr);
      return res;
    }
    @chpldoc.nodoc
    proc get(attr: string): owned Value throws do
      return this.get(owned Value, attr);

    /*
      Set an attribute/field of this Python object. This is equivalent to
      calling ``obj[attr] = value`` or ``setattr(obj, attr, value)`` in Python.

      :arg attr: The name of the attribute/field to set.
      :arg value: The value to set the attribute/field to.
    */
    proc set(attr: string, value) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyValue = interpreter.toPythonInner(value);
      defer Py_DECREF(pyValue);

      PyObject_SetAttrString(this.getPyObject(), attr.c_str(), pyValue);
      interpreter.checkException();
    }

    /*
      Call a method of this Python object. This is equivalent to calling
      ``obj.method(args)`` in Python.

      :arg retType: The Chapel type of the return value.
      :arg method: The name of the method to call.
      :arg args: The arguments to pass to the method.
      :arg kwargs: The keyword arguments to pass to the callable.
    */
    pragma "docs only"
    proc call(type retType = owned Value,
              method: string,
              const args...,
              kwargs:?=none): retType throws
              where kwargs.isAssociative() do compilerError("docs only");

    pragma "last resort"
    @chpldoc.nodoc
    proc call(method: string,
              type retType,
              const args...,
              kwargs:?=none): retType throws
              where kwargs.isAssociative() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArg = this.packTuple((...args));
      defer Py_DECREF(pyArg);

      var methodFunc =
        PyObject_GetAttrString(this.getPyObject(), method.c_str());
      this.interpreter.checkException();
      defer Py_DECREF(methodFunc);

      return callInternal(retType, methodFunc, pyArg, kwargs);
    }
    pragma "last resort"
    @chpldoc.nodoc
    proc call(method: string,const args...,
              kwargs:?=none): owned Value throws
              where kwargs.isAssociative() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArg = this.packTuple((...args));
      defer Py_DECREF(pyArg);

      var methodFunc =
        PyObject_GetAttrString(this.getPyObject(), method.c_str());
      this.interpreter.checkException();
      defer Py_DECREF(methodFunc);

      return callInternal(owned Value, methodFunc, pyArg, kwargs);
    }


    @chpldoc.nodoc
    proc call(type retType, method: string, const args...): retType throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArgs: args.size * PyObjectPtr;
      for param i in 0..#args.size {
        pyArgs(i) = interpreter.toPythonInner(args(i));
      }
      defer for pyArg in pyArgs do Py_DECREF(pyArg);

      var methodName = interpreter.toPythonInner(method);
      defer Py_DECREF(methodName);

      var pyRes = PyObject_CallMethodObjArgs(
        this.getPyObject(), methodName, (...pyArgs), nil);
      interpreter.checkException();

      var res = interpreter.fromPythonInner(retType, pyRes);
      return res;
    }
    @chpldoc.nodoc
    proc call(method: string, const args...): owned Value throws do
      return this.call(owned Value, method, (...args));

    @chpldoc.nodoc
    proc call(type retType, method: string): retType throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var methodName = interpreter.toPythonInner(method);
      defer Py_DECREF(methodName);

      var pyRes = PyObject_CallMethodNoArgs(this.getPyObject(), methodName);
      interpreter.checkException();

      var res = interpreter.fromPythonInner(retType, pyRes);
      return res;
    }
    @chpldoc.nodoc
    proc call(method: string): owned Value throws do
      return this.call(owned Value, method);


    pragma "last resort"
    @chpldoc.nodoc
    proc call(type retType,
              method: string,
              kwargs:?=none): retType throws where kwargs.isAssociative() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyArgs = Py_BuildValue("()");
      defer Py_DECREF(pyArgs);

      var methodFunc =
        PyObject_GetAttrString(this.getPyObject(), method.c_str());
      this.interpreter.checkException();
      defer Py_DECREF(methodFunc);

      return callInternal(retType, methodFunc, pyArgs, kwargs);
    }
    pragma "last resort"
    @chpldoc.nodoc
    proc call(method: string,
              kwargs:?=none): owned Value throws where kwargs.isAssociative() {
      return this.call(owned Value, method, kwargs=kwargs);
    }


    /*
      Returns the Chapel value of the object.

      This is a shortcut for calling :proc:`~Interpreter.fromPython` on this
      object, however it does not consume the object.
    */
    proc value(type value) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      // fromPython will decrement the ref count, so we need to increment it
      Py_INCREF(this.obj);
      return interpreter.fromPythonInner(value, this.obj);
    }

    /*
      Retain a new Python object.
      If the previous object was owned, decrements the reference count
    */
    @chpldoc.nodoc
    proc retain(val: PyObjectPtr, isOwned: bool = true) {
      var old = this.obj;
      this.obj = val;
      if this.isOwned {
        Py_DECREF(old);
      }
      this.isOwned = isOwned;
    }

    /*
      Stop owning ``val``, returns the underlying ``c_ptr(PyObject)``.

      The caller is responsible for decrementing the reference count of the
      returned object.
    */
    proc type release(in val: owned Value): PyObjectPtr {
      var valUn: unmanaged = owned.release(val);
      valUn.isOwned = false;
      var ptr: PyObjectPtr = valUn.obj;
      delete valUn;
      return ptr;
    }
    /*
      Create a new :type:`Value`, adopting the object.

      When the new :type:`Value` is deleted, the reference count of the object
      will be decremented.
    */
    proc type adopting(in interpreter: borrowed Interpreter,
                       in object: PyObjectPtr): owned Value throws {
      var val = new Value(interpreter, object, isOwned=true);
      return val;
    }
    /*
      Create a new :type:`Value`, adopting the object.

      When the new :type:`Value` is deleted, the reference count of the object
      will be untouched.
    */
    proc type borrowing(in interpreter: borrowed Interpreter,
                        in object: PyObjectPtr): owned Value throws {
      var val = new Value(interpreter, object, isOwned=false);
      return val;
    }

    /*
      Returns the :type:`~CTypes.c_ptr` to the underlying Python object.

      Most users should not need to call this method directly, except when
      calling low-level CPython functions.
    */
    proc getPyObject() do return this.obj;

    /*
      Iterate over an iterable Python object. This is equivalent to calling
      ``next`` continuously on an object until it raises ``StopIteration`` in
      Python.

      .. note::

         This iterator does not support parallelism.

      .. note::

         If iterating over a Python array, prefer using a :type:`PyArray` object
         and calling :iter:`PyArray.values` instead.
    */
    pragma "docs only"
    iter these(type eltType = owned Value): eltType throws do
      compilerError("docs only");


    @chpldoc.nodoc
    iter these(type eltType): eltType throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var iter_ = PyObject_GetIter(this.getPyObject());
      defer { if iter_ != nil then Py_DECREF(iter_); }
      if iter_ == nil || PyIter_Check(iter_) != 0 {
        throwChapelException("Object is not iterable");
      }
      while true {
        var item = PyIter_Next(iter_);
        interpreter.checkException();
        if item == nil {
          break;
        }
        yield interpreter.fromPythonInner(eltType, item);
      }
    }

    @chpldoc.nodoc
    iter these(): owned Value throws {
      try {
        // the try/catch is needed to work around a bug
        // https://github.com/chapel-lang/chapel/issues/27008
        for e in this.these(owned Value) do
          yield e;
      } catch e {
        throw e;
      }
    }


    // Casts
    /* Creates a new int from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: int(?)): t throws {
      return _castHelper(x, t);
    }

    /* Creates a new uint from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: uint(?)): t throws {
      return _castHelper(x, t);
    }

    /* Creates a new real from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: real(?)): t throws {
      return _castHelper(x, t);
    }

    /* Creates a new bool from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: bool): t throws {
      return _castHelper(x, t);
    }

    /* Creates a new bytes from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: bytes): t throws {
      return _castHelper(x, t);
    }

    /* Creates a new tuple from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: _tuple): t throws {
      return _castHelper(x, t);
    }

    /*
      Creates a new list from ``x``, when ``x`` is a :class:`Value`.
    */
    operator :(x: borrowed Value, type t: List.list(?)): t throws {
      return _castHelper(x, t);
    }

    /* Creates a new map from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: Map.map(?)): t throws {
      return _castHelper(x, t);
    }

    /* Creates a new set from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: Set.set(?)): t throws {
      return _castHelper(x, t);
    }

    /* Creates a new array from ``x``, when ``x`` is a :class:`Value`. */
    operator :(x: borrowed Value, type t: []): t throws {
      return _castHelper(x, t);
    }

    @chpldoc.nodoc
    proc type _castHelper(x: borrowed Value, type t): t throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyObj = x.getPyObject();
      Py_INCREF(pyObj);
      var res = x.interpreter.fromPythonInner(t, pyObj);

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
      Import a Python module by name. See :proc:`Interpreter.importModule`.
    */
    proc init(interpreter: borrowed Interpreter, in modName: string) {
      super.init(interpreter, nil: PyObjectPtr, isOwned=true);
      this.modName = modName;
      init this;
      this.obj = PyImport_ImportModule(modName.c_str());
    }


    /*
      Import a Python module from a string using an arbitrary name.
      See :proc:`Interpreter.createModule`.
    */
    proc init(interpreter: borrowed Interpreter,
              in modName: string, in moduleContents: string) throws {
      super.init(interpreter, nil: PyObjectPtr, isOwned=true);
      this.modName = modName;
      init this;
      // Only catch-less try! statements are allowed in initializers for now :(
      proc helper() throws {
        try {
          this.obj = interpreter.compileModule(moduleContents, modName);
        } catch e {
          // an exception thrown from the init will not result in a call to the postinit
          // or the deinit, so we have to handle that stuff here
          defer this.ctxInitState.exit();
          // rethrow the exception
          throw e;
        }
      }
      helper();
    }

    /*
      Import a Python module from bytecode using an arbitrary name.
      See :proc:`Interpreter.importModule`.
    */
    proc init(interpreter: borrowed Interpreter,
              in modName: string, in moduleBytecode: bytes) throws {
      super.init(interpreter, nil: PyObjectPtr, isOwned=true);
      this.modName = modName;
      init this;
      // Only catch-less try! statements are allowed in initializers for now :(
      proc helper() throws {
        try {
          this.obj = interpreter.compileModule(moduleBytecode, modName);
        } catch e {
          // an exception thrown from the init will not result in a call to the postinit
          // or the deinit, so we have to handle that stuff here
          defer this.ctxInitState.exit();
          // rethrow the exception
          throw e;
        }
      }
      helper();
    }
  }

  /*
    Represents a Python function.
  */
  class Function: Value {
    @chpldoc.nodoc
    var fnName: string;

    /*
      Get a handle to a function in a :class:`Value` by name.

      This is equivalent to ``mod.get(className)``. See :proc:`Value.get`.
    */
    proc init(mod: borrowed Value, in fnName: string) {
      super.init(mod.interpreter, nil: PyObjectPtr, isOwned=true);
      this.fnName = fnName;
      init this;
      this.obj = PyObject_GetAttrString(mod.getPyObject(), fnName.c_str());
    }
    /*
      Takes ownership of an existing Python object, pointed to by ``obj``.

      :arg interpreter: The interpreter that this object is associated with.
      :arg fnName: The name of the function.
      :arg obj: The :type:`~CTypes.c_ptr` to the existing object.
      :arg isOwned: Whether this object owns the Python object. This is true by
                    default.
    */
    proc init(interpreter: borrowed Interpreter,
              in fnName: string, in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
      this.fnName = fnName;
    }
    /*
      Create a new Python lambda function from a string. The lambda arguments
      must have a trailing comma.

      For example, to create a lambda function that takes two arguments, use:

      .. code-block:: python

         new Function(interpreter, "lambda x, y,: x + y")

      See also :proc:`Interpreter.compileLambda`.

      :arg interpreter: The interpreter that this object is associated with.
      :arg lambdaFn: The lambda function to create.
    */
    proc init(interpreter: borrowed Interpreter, in lambdaFn: string) throws {
      super.init(interpreter, nil: PyObjectPtr, isOwned=true);
      this.fnName = "<anon>";
      init this;
      // Only catch-less try! statements are allowed in initializers for now :(
      proc helper() throws {
        try {
          this.obj = interpreter.compileLambdaInternal(lambdaFn);
        } catch e {
          // an exception thrown from the init will not result in a call to the postinit
          // or the deinit, so we have to handle that stuff here
          defer this.ctxInitState.exit();
          // rethrow the exception
          throw e;
        }
      }
      helper();
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
      Get a handle to a class in a :class:`Value` by name.

      This is equivalent to ``mod.get(className)``. See :proc:`Value.get`.

      :arg mod: The module to get the class from.
      :arg className: The name of the class.
    */
    proc init(mod: borrowed Value, in className: string) {
      super.init(mod.interpreter, nil: PyObjectPtr, isOwned=true);
      this.className = className;
      init this;
      this.obj = PyObject_GetAttrString(mod.getPyObject(), className.c_str());
    }

    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
    }
  }

  private inline
  proc determineSliceBounds(bounds: range(?)): (Py_ssize_t, Py_ssize_t) {
    if (bounds.hasLowBound() && bounds.hasHighBound()) {
      return (bounds.low.safeCast(Py_ssize_t),
              bounds.high.safeCast(Py_ssize_t) + 1);

    } else if (!bounds.hasLowBound() && bounds.hasHighBound()) {
      return (min(Py_ssize_t), bounds.high.safeCast(Py_ssize_t) + 1);

    } else if (bounds.hasLowBound() && !bounds.hasHighBound()) {
      return (bounds.low.safeCast(Py_ssize_t), max(Py_ssize_t));

    } else {
      return (min(Py_ssize_t), max(Py_ssize_t));
    }
  }

  /*
    Represents a Python tuple.  This provides a Chapel interface to Python
    tuples, where the Python interpreter owns the tuple.
  */
  class PyTuple: Value {
    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
    }

    /*
      Get the size of the tuple. Equivalent to calling ``len(obj)`` in Python.

      :returns: The size of the tuple.
    */
    proc size: int throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var size = PyTuple_Size(this.getPyObject());
      this.interpreter.checkException();
      return size;
    }

    /*
      Get an item from the tuple. Equivalent to calling ``obj[idx]`` in Python.

      :arg T: The Chapel type of the item to return.
      :arg idx: The index of the item to get.
      :returns: The item at the given index.
    */
    pragma "docs only"
    proc get(type T = owned Value, idx: int): T throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc get(type T, idx: int): T throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var item = PyTuple_GetItem(this.getPyObject(), idx.safeCast(Py_ssize_t));
      this.interpreter.checkException();
      Py_INCREF(item);
      return interpreter.fromPythonInner(T, item);
    }

    @chpldoc.nodoc
    proc get(idx: int): owned Value throws do
      return this.get(owned Value, idx);

    /*
      Get a slice of the tuple indicated by ``bounds``.  Equivalent to
      calling ``obj[bounds.low:bounds.high+1]`` in Python.

      .. note::

         This method does not support strided ranges or ranges with an alignment
         other than 0.

      :arg T: the Chapel type of the slice to return.
      :arg bounds: The full slice of the tuple to return
      :returns: A slice of the tuple for the given bounds.
    */
    pragma "docs only"
    proc get(type T = owned Value, bounds: range(?)): T throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc get(type T, bounds: range(?)): T throws {
      if (bounds.strides != strideKind.one) {
        compilerError("cannot call `get()` on a Python tuple with a range with stride other than 1");
      }

      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyObj;

      var (low, high) = determineSliceBounds(bounds);
      pyObj = PyTuple_GetSlice(this.getPyObject(), low, high);
      this.interpreter.checkException();

      return interpreter.fromPythonInner(T, pyObj);
    }

    @chpldoc.nodoc
    proc get(bounds: range(?)): owned Value throws {
      if (bounds.strides != strideKind.one) {
        // Avoids the error from the other version reporting this function
        // instead of the user function
        compilerError("cannot call `get()` on a Python tuple with a range with stride other than 1");
      }
      return this.get(owned Value, bounds);
    }

    /*
      Check if an item is in the tuple.  Equivalent to calling ``item in obj``
      in Python.

      :arg item: The item to check for membership in the tuple.
    */
    proc contains(item: ?): bool throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var result = PySequence_Contains(this.getPyObject(),
                                       interpreter.toPythonInner(item));
      this.interpreter.checkException();
      return result: bool;
    }
  }

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
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var size = PySequence_Size(this.getPyObject());
      this.interpreter.checkException();
      return size;
    }

    /*
      Get an item from the list. Equivalent to calling ``obj[idx]`` in Python.

      :arg T: The Chapel type of the item to return.
      :arg idx: The index of the item to get.
      :returns: The item at the given index.
    */
    pragma "docs only"
    proc get(type T = owned Value, idx: int): T throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc get(type T, idx: int): T throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var item = PySequence_GetItem(this.getPyObject(),
                                    idx.safeCast(Py_ssize_t));
      this.interpreter.checkException();
      return interpreter.fromPythonInner(T, item);
    }
    @chpldoc.nodoc
    proc get(idx: int): owned Value throws do
      return this.get(owned Value, idx);

    /*
      Get a slice of the list indicated by ``bounds``.  Equivalent to
      calling ``obj[bounds.low:bounds.high+1]`` in Python.

      .. note::

         This method does not support strided ranges or ranges with an alignment
         other than 0.

      :arg T: the Chapel type of the slice to return.
      :arg bounds: The full slice of the list to return
      :returns: A slice of the list for the given bounds.
    */
    pragma "docs only"
    proc get(type T = owned Value, bounds: range(?)): T throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc get(type T, bounds: range(?)): T throws {
      if (bounds.strides != strideKind.one) {
        compilerError("cannot call `get()` on a Python list with a range with stride other than 1");
      }

      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyObj;

      var (low, high) = determineSliceBounds(bounds);
      pyObj = PyList_GetSlice(this.getPyObject(), low, high);
      this.interpreter.checkException();

      return interpreter.fromPythonInner(T, pyObj);
    }

    @chpldoc.nodoc
    proc get(bounds: range(?)): owned Value throws {
      if (bounds.strides != strideKind.one) {
        // Avoids the error from the other version reporting this function
        // instead of the user function
        compilerError("cannot call `get()` on a Python list with a range with stride other than 1");
      }
      return this.get(owned Value, bounds);
    }

    /*
      Set an item in the list. Equivalent to calling ``obj[idx] = item`` in
      Python.

      :arg idx: The index of the item to set.
      :arg item: The item to set.
    */
    proc set(idx: int, item: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PySequence_SetItem(this.getPyObject(),
                     idx.safeCast(Py_ssize_t),
                     interpreter.toPythonInner(item));
      this.interpreter.checkException();
    }

    /*
      Set an item in the list. Equivalent to calling
      ``obj[bounds.low:bounds.high+1] = items`` in Python.

      Note that providing more or less items than specified by `bounds` will
      change the length of the list.

      :arg bounds: The indices of the items to set.
      :arg item: The item to set.
    */
    proc set(bounds: range(?), items: ?) throws {
      if (bounds.strides != strideKind.one) {
        compilerError("cannot call `set()` on a Python list with a range with stride other than 1");
      }

      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var (low, high) = determineSliceBounds(bounds);
      PyList_SetSlice(this.getPyObject(), low, high,
                      interpreter.toPythonInner(items));
      this.interpreter.checkException();
    }

    /*
      Insert an item into the list at the specified index.  Equivalent to
      calling ``obj.insert(index, item)`` in Python.

      :arg idx: The index to insert the item at
      :arg item: The item to append
    */
    proc insert(idx: int, item: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PyList_Insert(this.getPyObject(), idx.safeCast(Py_ssize_t),
                    interpreter.toPythonInner(item));
      this.interpreter.checkException();
    }

    /*
      Append an item to the end of the list.  Equivalent to calling
      ``obj.append(item)`` in Python.

      :arg item: The item to append
    */
    proc append(item: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PyList_Append(this.getPyObject(), interpreter.toPythonInner(item));
      this.interpreter.checkException();
    }

    /*
      Extend the list with the contents of `iterable`.  Equivalent to calling
      ``obj.extend(iterable)`` in Python.

      .. note::

         This method is only support for Python 3.13 or later.  Calling it with
         earlier versions of Python will result in a PythonException that the
         method is not supported.

      :arg iterable: something that can be iterated over to extend the list
    */
    proc extend(iterable: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PyList_Extend(this.getPyObject(), interpreter.toPythonInner(iterable));
      this.interpreter.checkException();
    }

    /*
      Remove item at index from the list.  Equivalent to calling `del obj[idx]`
      in Python.

      :arg idx: The index of the item to remove.
    */
    proc remove(idx: int) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PyList_SetSlice(this.getPyObject(), idx.safeCast(Py_ssize_t),
                      idx.safeCast(Py_ssize_t) + 1, nil);
      this.interpreter.checkException();
    }

    /*
      Remove the specified items from the list.  Equivalent to calling
      ``obj[bounds.low:bounds.high+1] = []`` or
      ``del obj[bounds.low:bounds.high+1]`` in Python.

      :arg bounds: The indices of the items to remove.
    */
    proc remove(bounds: range(?)) throws {
      if (bounds.strides != strideKind.one) {
        compilerError("cannot call `remove()` on a Python list with a range with stride other than 1");
      }

      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var (low, high) = determineSliceBounds(bounds);
      PyList_SetSlice(this.getPyObject(), low, high, nil);
      this.interpreter.checkException();
    }

    /*
      Remove all items from the list.  Equivalent to calling ``obj.clear()`` or
      ``del obj[:]``

      .. note::

         This method is only support for Python 3.13 or later.  Calling it with
         earlier versions of Python will result in a PythonException that the
         method is not supported.
    */
    proc clear() throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PyList_Clear(this.getPyObject());
      this.interpreter.checkException();
    }
  }

  /*
    Represents a Python dict. This provides a Chapel interface to Python dicts,
    where the Python interpreter owns the dict.
  */
  class PyDict: Value {
    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
    }

    /*
      Get the size of the dict. Equivalent to calling ``len(obj)`` in Python.

      :returns: The size of the dict.
    */
    proc size: int throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var size = PyDict_Size(this.getPyObject());
      this.interpreter.checkException();
      return size;
    }

    /*
      Get an item from the dict. Equivalent to calling ``obj[key]`` in Python.

      :arg T: The Chapel type of the item to return.
      :arg key: The key of the item to get.
      :returns: The item at the given key.
    */
    pragma "docs only"
    proc get(type T = owned Value, key: ?): T throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc get(type T, key: ?): T throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var item = PyDict_GetItem(this.getPyObject(),
                                interpreter.toPythonInner(key));
      this.interpreter.checkException();
      Py_INCREF(item);
      return interpreter.fromPythonInner(T, item);
    }
    @chpldoc.nodoc
    proc get(key: ?): owned Value throws do
      return this.get(owned Value, key);

    /*
      Set an item in the dict. Equivalent to calling ``obj[key] = item`` in
      Python.

      :arg key: The key of the item to set.
      :arg item: The item to set.
    */
    proc set(key: ?, item: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var val = interpreter.toPythonInner(item);
      PyDict_SetItem(this.getPyObject(),
                     interpreter.toPythonInner(key),
                     val);
      Py_DECREF(val);
      this.interpreter.checkException();
    }

    /*
      Delete an item from the dict.  Equivalent to calling ``del obj[key]`` in
      Python.

      :arg key: The key of the item to delete.

      :throws KeyError: If the key did not exist in the dict.
    */
    proc del(key: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PyDict_DelItem(this.getPyObject(), interpreter.toPythonInner(key));
      this.interpreter.checkException();
    }

    /*
      Remove all elements from the dict.  Equivalent to calling ``obj.clear()``
      in Python.
    */
    proc clear() throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PyDict_Clear(this.getPyObject());
      this.interpreter.checkException();
    }

    /*
      Perform a shallow copy into a new dict.  Equivalent to calling
      ``obj.copy()`` in Python.
    */
    proc copy(): PyDict throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var c = PyDict_Copy(this.getPyObject());
      this.interpreter.checkException();
      return new PyDict(this.interpreter, c);
    }

    /*
      Check if an item is in the dict.  Equivalent to calling ``item in obj`` in
      Python.

      :arg key: The key to check for membership in the dict.
    */
    proc contains(key: ?): bool throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var result = PyDict_Contains(this.getPyObject(),
                                   interpreter.toPythonInner(key));
      this.interpreter.checkException();
      return result: bool;
    }
  }

  /*
    Represents a Python set. This provides a Chapel interface to Python sets,
    where the Python interpreter owns the set.
  */
  class PySet: Value {
    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
    }

    /*
      Get the size of the set. Equivalent to calling ``len(obj)`` in Python.

      :returns: The size of the set.
    */
    proc size: int throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var size = PySequence_Size(this.getPyObject());
      this.interpreter.checkException();
      return size;
    }

    /*
      Add an item to the set.  Equivalent to calling ``obj.add(item)`` in
      Python.

      :arg item: The item to add to the set.
     */
    proc add(item: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PySet_Add(this.getPyObject(), interpreter.toPythonInner(item));
      this.interpreter.checkException();
    }

    /*
      Check if an item is in the set.  Equivalent to calling ``item in obj`` in
      Python.

      :arg item: The item to check for membership in the set.
    */
    proc contains(item: ?): bool throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var result = PySet_Contains(this.getPyObject(),
                                  interpreter.toPythonInner(item));
      this.interpreter.checkException();
      return result: bool;
    }

    /*
      Discard a specific item from the set.  Equivalent to calling
      ``obj.discard(item)`` in Python.

      :arg item: The item to discard from the set.
    */
    proc discard(item: ?) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PySet_Discard(this.getPyObject(),
                    interpreter.toPythonInner(item));
      this.interpreter.checkException();
    }

    /*
      Pop an arbitrary element from the set and return it.  Equivalent to
      calling ``obj.pop()`` in Python.

      :arg T: The expected type of the element popped.  Defaults to
              :type:`Value`.  If the Python set contains only elements of a
              single type, that type can be specified using this argument.
              Otherwise, `Value` is the most appropriate type, as we do not
              control which element `pop` will return.
    */
    proc pop(type T = owned Value): T throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var popped = PySet_Pop(this.getPyObject());
      this.interpreter.checkException();
      return interpreter.fromPythonInner(T, popped);
    }

    /*
      Remove all elements from the set.  Equivalent to calling ``obj.clear()``
      in Python
    */
    proc clear() throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PySet_Clear(this.getPyObject());
      this.interpreter.checkException();
    }
  }

  private proc checkFormatWithEltType(format: c_ptr(c_char),
                              itemSize: Py_ssize_t, type eltType): bool {
    // we require format to be set, it should only be unset if the buffer
    // producer does not conform to the standard
    if format == nil then return false;
    // See https://docs.python.org/3/library/struct.html#format-characters
    // and https://docs.python.org/3/library/array.html for the list of format
    // characters
    // Python defines minimum bitwidths, not exact bitwidths, which means
    // if we solely rely on the format string we may have portability issues
    // so rely solely on itemsize, and use the format string to help with
    // signedness. Bools are a special case, as Chapel does not define a
    // specific bitwidth for bools
    if eltType == bool {
      return format.deref() == "?".toByte();
    } else {
      if numBytes(eltType) != itemSize {
        return false;
      }
      if isIntegralType(eltType) {
        // the signedness of 'c' is implementation defined, so no need to check
        if format.deref() == 'c'.toByte() then return true;

        var isSigned = format.deref() == 'b'.toByte() ||
                       format.deref() == 'h'.toByte() ||
                       format.deref() == 'i'.toByte() ||
                       format.deref() == 'l'.toByte() ||
                       format.deref() == 'q'.toByte() ||
                       format.deref() == 'n'.toByte();
        if isSigned && isIntType(eltType) then
          return true;

        var isUnsigned = format.deref() == 'B'.toByte() ||
                         format.deref() == 'H'.toByte() ||
                         format.deref() == 'I'.toByte() ||
                         format.deref() == 'L'.toByte() ||
                         format.deref() == 'Q'.toByte() ||
                         format.deref() == 'N'.toByte();
        return isUnsigned && isUintType(eltType);
      } else if isRealType(eltType) || isImagType(eltType) {
        var isFP = format.deref() == 'f'.toByte() ||
                   format.deref() == 'd'.toByte() ||
                  format.deref() == 'e'.toByte();
        return isFP;
      } else {
        return false;
      }
    }
  }

  /*
    Represents a Python array. This provides a Chapel interface to Python types
    that are array-like. This includes
    `array.array <https://docs.python.org/3/library/array.html>`_ and
    `numpy.ndarray <https://numpy.org/doc/stable/reference/generated/numpy.ndarray.html>`_.
  */
  class PyArray: Value {
    /*
      The Chapel type of the elements in the array. If this is left unspecified,
      i.e. ``nothing``, certain operations will require the user to specify the
      type at compile time.
    */
    type eltType = nothing;
    /*
      The number of dimensions of the array. If this is left unspecified, i.e.
      ``-1``, certain operations will require the user to specify the rank at
      compile time.
    */
    param rank: int = -1;

    @chpldoc.nodoc
    var view: Py_buffer;

    @chpldoc.nodoc
    proc init(type eltType = nothing, param rank: int = -1) {
      this.eltType = eltType;
      this.rank = rank;
    }

    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter,
              in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
    }
    @chpldoc.nodoc
    proc init(type eltType, param rank: int,
              in interpreter: borrowed Interpreter,
              in obj: PyObjectPtr, isOwned: bool = true) {
      super.init(interpreter, obj, isOwned=isOwned);
      this.eltType = eltType;
      this.rank = rank;
    }

    @chpldoc.nodoc
    proc postinit() throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      if PyObject_CheckBuffer(this.getPyObject()) == 0 {
        throw new ChapelException("Object does not support buffer protocol");
      }
      const flags = PyBUF_SIMPLE | PyBUF_WRITABLE | PyBUF_FORMAT |
                    PyBUF_ND | PyBUF_STRIDES | PyBUF_C_CONTIGUOUS;
      if PyObject_GetBuffer(this.getPyObject(),
                            c_ptrTo(this.view), flags) == -1 {
        this.interpreter.checkException();
        // this.check should have raised an exception, if it didn't, raise one
        throw new BufferError("Failed to get buffer");
      }
      if this.view.ndim == 0 {
        throw new ChapelException("0-dimensional arrays are not supported");
      }
      if this.view.shape == nil || this.view.strides == nil {
        throw new ChapelException(
          "The Python array does not properly support the buffer protocol"
        );
      }
    }

    @chpldoc.nodoc
    proc deinit() {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      PyBuffer_Release(c_ptrTo(this.view));
    }


    /*
      The number of dimensions of the Python array.
    */
    proc ndim: int do return this.view.ndim;

    proc size: int {
      // equivalent check is `this.view.len / this.view.itemsize`
      var s = 1;
      for i in 0..<this.view.ndim {
        s *= this.view.shape(i);
      }
      return s;
    }

    /*
      The shape of the Python array, which is a tuple of integers that
      represent the size of each dimension of the array.

      :arg rank: The number of dimensions of the array. This must be known at
                 compile time and match the rank of the Python array at runtime.
      :returns: A tuple of integers representing the shape of the array.
    */
    pragma "docs only"
    proc shape(param rank: int = this.rank): rank*int throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc shape(param rank: int): rank*int throws {
      if rank == -1 then
        compilerError("Rank must be specified at compile time");

      if boundsChecking then
        if rank != this.view.ndim then
          throw new ChapelException(
            "Rank mismatch: expected " + rank:string +
            " dimensions, but got " + this.view.ndim:string);

      var s: rank*int;
      for param i in 0..<rank {
        s(i) = this.view.shape(i);
      }
      return s;
    }
    @chpldoc.nodoc
    proc shape() throws where this.rank == -1 {
      compilerError("Rank must be specified at compile time");
    }
    @chpldoc.nodoc
    proc shape(): this.rank*int throws where this.rank != -1 do
      return this.shape(this.rank);

    /*
      Get an element from the Python array. This results in a modifiable
      reference to the element of the Python array. The index must be either a
      single `int` (for 1-D arrays) or a tuple of `int`'s where the number of
      indices is equal to the number of dimensions of the array. This method
      does not currently support slicing.

      .. warning::

          This method performs bounds checking and will throw if the index is
          out of bounds. This is a runtime check that is turned off with
          ``--no-checks``.

      ``eltType`` can be optionally specified to override the default value for
      the ``PyArray`` object. If ``eltType`` is fully specified in the
      ``PyArray`` object, it does not need to be specified here.
    */
    pragma "docs only"
    proc this(type eltType = this.eltType, idx) ref : eltType throws {
      compilerError("docs only");
    }

    @chpldoc.nodoc
    proc isValidArrayIndex(idx) param : bool do
      return idx.type == int ||
             (isHomogeneousTupleType(idx.type) && idx(0).type == int);

    @chpldoc.nodoc
    proc this(idx) ref : eltType throws where isValidArrayIndex(idx) do
      return this(eltType, idx);

    @chpldoc.nodoc
    proc this(type eltType, idx) ref : eltType throws
    where isValidArrayIndex(idx) {

      if eltType == nothing then
        compilerError("Element type must be specified at compile time");
      if this.rank != -1 {
        if isHomogeneousTupleType(idx.type) && this.rank != idx.size then
          compilerError("Attempting to index an array of rank " +
                        this.rank:string + " with a " + idx.size:string +
                        "-dimensional index");
        if idx.type == int && this.rank != 1 then
          compilerError("Attempting to index an array of rank " +
                        this.rank:string + " with a 1-dimensional index");
      }


      if boundsChecking then
        if !checkFormatWithEltType(this.view.format,
                                  this.view.itemsize, eltType) {
          throw new ChapelException(
            "Source array's format does not match requested element type"
          );
        }

      var offset = 0;
      if idx.type == int {
        if boundsChecking then
          if this.view.ndim != 1 {
            throw new ChapelException("Cannot index a " +
              this.view.ndim:string + "-dimensional array with a " +
              "1-dimensional index");
          }
        offset = idx * this.view.strides(0);
      } else {
        if boundsChecking then
          if idx.size != this.view.ndim {
            throw new ChapelException("Cannot index a " +
              this.view.ndim:string + "-dimensional array with a " +
              idx.size:string + "-dimensional index");
          }
        for param i in 0..<idx.size {
          offset += idx(i) * this.view.strides(i);
        }
      }

      if boundsChecking then
        if offset < 0 || offset >= this.view.len {
          throw new ChapelException("Index out of bounds");
        }

      var ptr_ =
        (this.view.buf:c_intptr + offset:c_intptr): c_ptr(void): c_ptr(eltType);
      return ptr_.deref();
    }

    @chpldoc.nodoc
    proc this(idx) ref : eltType throws where !isValidArrayIndex(idx) do
      compilerError(
        "Invalid index type of '" + idx.type:string + "' for array - " +
        "index must be a single int (for 1D arrays only) " +
        "or a tuple of ints");

    @chpldoc.nodoc
    proc this(type eltType, idx) ref : eltType throws
    where !isValidArrayIndex(idx) do
      compilerError(
        "Invalid index type of '" + idx.type:string + "' for array - " +
        "index must be a single int (for 1D arrays only) " +
        "or a tuple of ints");


    /*
      Iterate over the elements of the Python array.

      .. warning::

        This invokes the Python iterator, which has considerable overhead.
        Prefer using :iter:`PyArray.values` which is significantly faster and
        supports parallel iteration.
    */
    pragma "docs only"
    override iter these(type eltType = owned Value): eltType throws do
      compilerError("docs only");
    //
    // TODO: these are meant to prevent users from calling .these on a PyArray
    // when they probaby wanted .values. But the mere presence of these as
    // compiler errors prevents any program using Value.these from
    // compiling. And we can't just make them throw instead because inheritance
    // prevents iterator inlining, which is not yet supported
    //
    // @chpldoc.nodoc
    // override iter these(type eltType): eltType throws {
    //   compilerError(
    //     "Calling '.these(eltType)' on a PyArray is not supported."
    //     + " Use '.values(eltType)' instead.");
    // }
    // @chpldoc.nodoc
    // override iter these(): eltType throws {
    //   compilerError(
    //     "Calling '.these()' on a PyArray is not supported."
    //     + " Use '.values()' instead.");
    // }

    /*
      Iterate over the elements of the Python array. This results in a Chapel
      iterator that yields the elements of the Python array. This yields
      modifiable references to the elements of the Python array.

      ``eltType`` can be optionally specified to override the default value for
      the ``PyArray`` object. If ``eltType`` is fully specified in the
      ``PyArray`` object, it does not need to be specified here.
    */
    pragma "docs only"
    iter values(type eltType = this.eltType) ref : eltType throws {
      compilerError("docs only");
    }

    @chpldoc.nodoc
    iter values(type eltType) ref : eltType throws {
      if eltType == nothing then
        compilerError("Element type must be specified at compile time");

      if boundsChecking then
        if !checkFormatWithEltType(this.view.format,
                                  this.view.itemsize, eltType) {
          throw new ChapelException(
            "Source array's format does not match requested element type"
          );
        }
      var ptr_ = this.view.buf: c_ptr(eltType);
      foreach i in 0..#this.size {
        yield ptr_[i];
      }
    }
    @chpldoc.nodoc
    iter values() ref : eltType throws {
      for e in values(eltType=this.eltType) do yield e;
    }
    // TODO: it should also be possible to support leader/follower here
    @chpldoc.nodoc
    iter values(param tag: iterKind, type eltType) ref : eltType throws
     where tag == iterKind.standalone {
      if eltType == nothing then
        compilerError("Element type must be specified at compile time");

      if boundsChecking then
        if !checkFormatWithEltType(this.view.format,
                                  this.view.itemsize, eltType) {
          throw new ChapelException(
            "Source array's format does not match requested element type"
          );
        }
      var ptr_ = this.view.buf: c_ptr(eltType);
      forall i in 0..#this.size {
        yield ptr_[i];
      }
    }
    @chpldoc.nodoc
    iter values(param tag: iterKind) ref : eltType throws
    where tag == iterKind.standalone do
      foreach e in values(tag=tag, eltType=this.eltType) do yield e;

    /*
      Get the Chapel array from the Python array. This results in a Chapel view
      of the underlying data in the Python array. The data is not copied, so
      modifying the Chapel array will modify the Python array and vice versa.

      ``eltType`` and ``rank`` can be optionally specified to override the
      default values for the ``PyArray`` object. If ``eltType`` and ``rank``
      are fully specified in the ``PyArray`` object, they do not need to be
      specified here.
    */
    pragma "docs only"
    proc array(type eltType = this.eltType,
               param rank: int = this.rank): [] throws do
      compilerError("docs only");

    @chpldoc.nodoc
    proc array(): [] throws do return array(this.eltType, this.rank);
    @chpldoc.nodoc
    proc array(type eltType): [] throws do return array(eltType, this.rank);
    @chpldoc.nodoc
    proc array(param rank: int): [] throws do return array(this.eltType, rank);

    @chpldoc.nodoc
    proc array(type eltType, param rank: int): [] throws {

      if eltType == nothing then
        compilerError("Element type must be specified at compile time");
      if rank == -1 then
        compilerError("Rank must be specified at compile time");

      if boundsChecking then
        if !checkFormatWithEltType(this.view.format,
                                  this.view.itemsize, eltType) {
          throw new ChapelException(
            "Source array's format does not match requested element type"
          );
        }
      var buf = this.view.buf: c_ptr(eltType);
      var ndim = this.view.ndim;

      if boundsChecking then
        if ndim != rank {
          throw new ChapelException(
            "Python array of rank " + ndim:string +
            " cannot be converted to a Chapel array of rank " + rank:string);
        }

      var ranges: rank * range(int, boundKind.both, strideKind.any);
      for param i in 0..<rank {
        ranges(i) = 0.. # this.view.shape(i);
      }

      var dom = chpl__buildDomainExpr((...ranges), false);
      var res = makeArrayFromPtr(buf, dom);
      return res;
    }
  }

  // TODO: this could be generalized with
  // https://github.com/chapel-lang/chapel/issues/26958
  pragma "last resort"
  @chpldoc.nodoc
  operator:(x: borrowed PyArray(?), type t: PyArray(?)): t throws do
    return Value._castHelper(x, t);

  @chpldoc.nodoc
  proc isSupportedArrayType(arr) param : bool {
    return isArrayType(arr.type) &&
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
       :proc:`~Array.get` do no translation of the domain and should be
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
    proc init(in interpreter: borrowed Interpreter, ref arr: [])
      where isSupportedArrayType(arr) {
      super.init(interpreter, nil: PyObjectPtr, isOwned=true);
      this.eltType = arr.eltType;
      init this;
      this.obj = ArrayTypes.createArray(arr);
    }
    @chpldoc.nodoc
    proc init(in interpreter: borrowed Interpreter, ref arr: [])
      where !isSupportedArrayType(arr) {
      super.init(interpreter, nil: PyObjectPtr, isOwned=false);
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
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var size = PySequence_Size(this.getPyObject());
      this.interpreter.checkException();
      return size;
    }

    /*
      Get an item from the array. Equivalent to calling ``obj[idx]`` in Python
      or ``originalArray[idx]`` in Chapel.

      :arg idx: The index of the item to get.
      :returns: The item at the given index.
    */
    proc get(idx: int): eltType throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyObj = PySequence_GetItem(this.getPyObject(),
                                     idx.safeCast(Py_ssize_t));
      this.interpreter.checkException();
      return interpreter.fromPythonInner(eltType, pyObj);
    }
    /*
      Set an item in the array. Equivalent to calling ``obj[idx] = item`` in
      Python or ``originalArray[idx] = item`` in Chapel.

      :arg idx: The index of the item to set.
      :arg item: The item to set.
    */
    proc set(idx: int, item: eltType) throws {
      var ctx = chpl_pythonContext.enter();
      defer ctx.exit();

      var pyItem = interpreter.toPythonInner(item);
      PySequence_SetItem(this.getPyObject(), idx.safeCast(Py_ssize_t), pyItem);
      this.interpreter.checkException();
    }
  }


  // TODO: now that we handle this on every call to the python interpreter,
  // is this still needed as a user facing type?
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

  // TODO: now that we handle this on every call to the python interpreter,
  // is this still needed as a user facing type?
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
  PyTuple implements writeSerializable;
  PyList implements writeSerializable;
  PyDict implements writeSerializable;
  PySet implements writeSerializable;
  Array implements writeSerializable;
  PyArray implements writeSerializable;
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
  override proc PyTuple.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc PyList.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc PyDict.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc PySet.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc Array.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  override proc PyArray.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  @chpldoc.nodoc
  proc NoneType.serialize(writer, ref serializer) throws do
    writer.write(this:string);

  //
  // binary ops
  //
  private inline proc _binaryOp(param op: string,
                                lhs: borrowed Value,
                                rhs: borrowed Value): owned Value throws {
    var ctx = chpl_pythonContext.enter();
    defer ctx.exit();
    var res = PyObject_CallMethod(
      lhs.interpreter.operatorModule, op,
      "OO", lhs.getPyObject(), rhs.getPyObject());
    lhs.interpreter.checkException();
    return new Value(lhs.interpreter, res, isOwned=true);
  }
  private inline proc _binaryOpInPlace(param op: string,
                                       lhs: borrowed Value,
                                       rhs: borrowed Value) throws {
    var ctx = chpl_pythonContext.enter();
    defer ctx.exit();
    var res = PyObject_CallMethod(
      lhs.interpreter.operatorModule, op,
      "OO", lhs.getPyObject(), rhs.getPyObject());
    lhs.interpreter.checkException();
    lhs.retain(res);
    lhs.interpreter.checkException();
  }
  operator +(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("add", lhs, rhs);
  operator +=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("iadd", lhs, rhs);
  operator +=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("iadd", lhs.borrow(), rhs.borrow());

  operator -(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("sub", lhs, rhs);
  operator -=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("isub", lhs, rhs);
  operator -=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("isub", lhs.borrow(), rhs.borrow());

  operator *(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("mul", lhs, rhs);
  operator *=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("imul", lhs, rhs);
  operator *=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("imul", lhs.borrow(), rhs.borrow());

  operator /(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("truediv", lhs, rhs);
  operator /=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("itruediv", lhs, rhs);
  operator /=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("itruediv", lhs.borrow(), rhs.borrow());

  operator %(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("mod", lhs, rhs);
  operator %=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("imod", lhs, rhs);
  operator %=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("imod", lhs.borrow(), rhs.borrow());

  operator **(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("pow", lhs, rhs);
  operator **=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("ipow", lhs, rhs);
  operator **=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("ipow", lhs.borrow(), rhs.borrow());

  operator &(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("and_", lhs, rhs);
  operator &=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("iand", lhs, rhs);
  operator &=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("iand", lhs.borrow(), rhs.borrow());

  operator |(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("or_", lhs, rhs);
  operator |=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("ior", lhs, rhs);
  operator |=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("ior", lhs.borrow(), rhs.borrow());

  operator ^(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("xor", lhs, rhs);
  operator ^=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("ixor", lhs, rhs);
  operator ^=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("ixor", lhs.borrow(), rhs.borrow());

  operator <<(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("lshift", lhs, rhs);
  operator <<=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("ilshift", lhs, rhs);
  operator <<=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("ilshift", lhs.borrow(), rhs.borrow());

  operator >>(lhs: borrowed Value, rhs: borrowed Value): owned Value throws do
    return _binaryOp("rshift", lhs, rhs);
  operator >>=(ref lhs: owned Value, rhs: borrowed Value) throws do
    _binaryOpInPlace("irshift", lhs, rhs);
  operator >>=(ref lhs: owned Value, const ref rhs: Value) throws do
    _binaryOpInPlace("irshift", lhs.borrow(), rhs.borrow());

  //
  // unary ops
  //
  private inline proc _unaryOp(param op: string,
                               lhs: borrowed Value): owned Value throws {
    var ctx = chpl_pythonContext.enter();
    defer ctx.exit();
    var res = PyObject_CallMethod(
      lhs.interpreter.operatorModule, op,
      "O", lhs.getPyObject());
    lhs.interpreter.checkException();
    return new Value(lhs.interpreter, res, isOwned=true);
  }
  operator +(lhs: borrowed Value): owned Value throws do
    return _unaryOp("pos", lhs);
  operator -(lhs: borrowed Value): owned Value throws do
    return _unaryOp("neg", lhs);
  operator ~(lhs: borrowed Value): owned Value throws do
    return _unaryOp("invert", lhs);
  operator !(lhs: borrowed Value): owned Value throws do
    return _unaryOp("not_", lhs);

  //
  // comparison ops
  //
  private inline proc _cmpOp(param op: string,
                            lhs: borrowed Value,
                            rhs: borrowed Value): bool throws {
    var ctx = chpl_pythonContext.enter();
    defer ctx.exit();
    var res = PyObject_CallMethod(
      lhs.interpreter.operatorModule, op,
      "OO", lhs.getPyObject(), rhs.getPyObject());
    lhs.interpreter.checkException();
    return lhs.interpreter.fromPythonInner(bool, res);
  }
  operator ==(lhs: borrowed Value, rhs: borrowed Value): bool throws do
    return _cmpOp("eq", lhs, rhs);
  operator !=(lhs: borrowed Value, rhs: borrowed Value): bool throws do
    return _cmpOp("ne", lhs, rhs);

  operator <(lhs: borrowed Value, rhs: borrowed Value): bool throws do
    return _cmpOp("lt", lhs, rhs);
  operator <=(lhs: borrowed Value, rhs: borrowed Value): bool throws do
    return _cmpOp("le", lhs, rhs);

  operator >(lhs: borrowed Value, rhs: borrowed Value): bool throws do
    return _cmpOp("gt", lhs, rhs);
  operator >=(lhs: borrowed Value, rhs: borrowed Value): bool throws do
    return _cmpOp("ge", lhs, rhs);


  @chpldoc.nodoc
  module CWChar {
    require "wchar.h";
    private use CTypes;

    /*
      Convert a Chapel string to a `wchar_t*`, this is the same as `.c_str()`

      This allocates a new buffer, the caller is responsible for freeing it.
    */
    proc string.c_wstr(): c_ptr(c_wchar_t) {
      extern proc mbstowcs(dest: c_ptr(c_wchar_t),
                           src: c_ptrConst(c_char),
                           n: c_size_t): c_size_t;
      var len: c_size_t = this.size.safeCast(c_size_t);
      var buf = allocate(c_wchar_t, len + 1, clear=true);
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
    extern type PyInterpreterState;
    type PyInterpreterStatePtr = c_ptr(PyInterpreterState);

    /*
      PyConfig
    */
    extern record PyConfig {
      var isolated: c_int;
      var use_environment: c_int;
      var filesystem_encoding: c_ptr(c_wchar_t);
      var site_import: c_int;
      var user_site_directory: c_int;
      var program_name: c_ptr(c_wchar_t);
      var home: c_ptr(c_wchar_t);
      var module_search_paths_set: c_int;
      var module_search_paths: PyWideStringList;
      var executable: c_ptr(c_wchar_t);
    }
    extern proc PyConfig_Clear(config_: c_ptr(PyConfig));
    extern proc PyConfig_Read(config_: c_ptr(PyConfig)): PyStatus;
    extern proc PyConfig_InitIsolatedConfig(config_: c_ptr(PyConfig));
    extern proc PyConfig_InitPythonConfig(config_: c_ptr(PyConfig));

    extern proc PyConfig_SetString(config_: c_ptr(PyConfig),
                                   config_str: c_ptr(c_ptr(c_wchar_t)),
                                   str: c_ptr(c_wchar_t)): PyStatus;
    extern proc PyConfig_SetBytesString(config_: c_ptr(PyConfig),
                                        config_str: c_ptr(c_ptr(c_wchar_t)),
                                        str: c_ptrConst(c_char)): PyStatus;

    extern type PyWideStringList;
    extern proc PyWideStringList_Append(list: c_ptr(PyWideStringList),
                                        str: c_ptr(c_wchar_t)): PyStatus;
    extern proc PyWideStringList_Insert(list: c_ptr(PyWideStringList),
                                        idx: c_int,
                                        str: c_ptr(c_wchar_t)): PyStatus;

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
    extern proc PyObject_Repr(obj: PyObjectPtr): PyObjectPtr; // `repr(obj)`
    extern proc PyObject_Type(obj: PyObjectPtr): PyObjectPtr; // `type(obj)`
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
    extern proc chpl_PyEval_GetFrameBuiltins(): PyObjectPtr;

    extern var Py_eval_input: c_int;
    extern var Py_file_input: c_int;
    extern proc PyRun_String(code: c_ptrConst(c_char), start: c_int,
                             globals: PyObjectPtr,
                             locals: PyObjectPtr): PyObjectPtr;
    extern proc chpl_Py_CompileString(code: c_ptrConst(c_char),
                                      filename: c_ptrConst(c_char),
                                      start: c_int): PyObjectPtr;
    extern proc PyImport_ExecCodeModule(name: c_ptrConst(c_char),
                                        code: PyObjectPtr): PyObjectPtr;
    extern proc PyMarshal_ReadObjectFromString(data: c_ptrConst(c_char),
                                               size: Py_ssize_t): PyObjectPtr;
    extern proc PyImport_AddModule(name: c_ptrConst(c_char)): PyObjectPtr;
    extern proc PyModule_GetDict(mod: PyObjectPtr): PyObjectPtr;

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
    extern const PyExc_KeyError: PyObjectPtr;
    extern const PyExc_BufferError: PyObjectPtr;
    extern const PyExc_NotImplementedError: PyObjectPtr;

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

    inline proc Py_None: PyObjectPtr {
      extern proc chpl_Py_None(): PyObjectPtr;
      return chpl_Py_None();
    }
    inline proc Py_True: PyObjectPtr {
      extern proc chpl_Py_True(): PyObjectPtr;
      return chpl_Py_True();
    }
    inline proc Py_False: PyObjectPtr {
      extern proc chpl_Py_False(): PyObjectPtr;
      return chpl_Py_False();
    }
    inline proc Py_NotImplemented: PyObjectPtr {
      extern proc chpl_Py_NotImplemented(): PyObjectPtr;
      return chpl_Py_NotImplemented();
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
    extern proc PySequence_Contains(obj: PyObjectPtr,
                                    item: PyObjectPtr): c_int;


    /*
      Iterators
    */
    extern proc PyObject_GetIter(obj: PyObjectPtr): PyObjectPtr;
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
    extern proc PyList_SetSlice(list: PyObjectPtr, low: Py_ssize_t,
                                high: Py_ssize_t, itemlist: PyObjectPtr);
    extern proc PyList_GetItem(list: PyObjectPtr, idx: Py_ssize_t): PyObjectPtr;
    extern proc PyList_GetSlice(list: PyObjectPtr, low: Py_ssize_t,
                                high: Py_ssize_t): PyObjectPtr;
    extern proc PyList_Size(list: PyObjectPtr): Py_ssize_t;
    extern proc PyList_Insert(list: PyObjectPtr,
                              idx: Py_ssize_t,
                              item: PyObjectPtr);
    extern proc PyList_Append(list: PyObjectPtr, item: PyObjectPtr);
    extern "chpl_PyList_Clear" proc PyList_Clear(list: PyObjectPtr);
    extern "chpl_PyList_Extend" proc PyList_Extend(list: PyObjectPtr,
                                                   iterable: PyObjectPtr);

    /*
      Sets
    */
    extern proc PySet_Check(set: PyObjectPtr): c_int;
    extern proc PySet_New(set: PyObjectPtr): PyObjectPtr;
    extern proc PySet_Add(set: PyObjectPtr, item: PyObjectPtr);
    extern proc PySet_Size(set: PyObjectPtr): Py_ssize_t;
    extern proc PySet_Contains(set: PyObjectPtr, item: PyObjectPtr): c_int;
    extern proc PySet_Discard(set: PyObjectPtr, item: PyObjectPtr): c_int;
    extern proc PySet_Pop(set: PyObjectPtr): PyObjectPtr;
    extern proc PySet_Clear(set: PyObjectPtr): c_int;


    /*
      Dictionaries
    */
    extern proc PyDict_New(): PyObjectPtr;
    extern proc PyDict_Contains(dict: PyObjectPtr, key: PyObjectPtr): c_int;
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
    extern proc PyDict_DelItem(dict: PyObjectPtr,
                               key: PyObjectPtr);
    extern proc PyDict_Size(dict: PyObjectPtr): Py_ssize_t;
    extern proc PyDict_Clear(dict: PyObjectPtr);
    extern proc PyDict_Copy(dict: PyObjectPtr): PyObjectPtr;
    extern proc PyDict_Keys(dict: PyObjectPtr): PyObjectPtr;
    extern proc PyDict_DelItemString(dict: PyObjectPtr,
                                     key: c_ptrConst(c_char));

    extern proc PyObject_GetAttrString(obj: PyObjectPtr,
                                       name: c_ptrConst(c_char)): PyObjectPtr;
    extern proc PyObject_SetAttrString(obj: PyObjectPtr,
                                       name: c_ptrConst(c_char),
                                       value: PyObjectPtr);
    extern proc PyObject_HasAttrString(obj: PyObjectPtr,
                                       name: c_ptrConst(c_char)): c_int;

    /*
      Tuples
    */
    extern proc PyTuple_New(size: Py_ssize_t): PyObjectPtr;
    extern proc PyTuple_Pack(size: Py_ssize_t, args...): PyObjectPtr;
    extern proc PyTuple_Size(tup: PyObjectPtr): Py_ssize_t;
    extern proc PyTuple_GetItem(tup: PyObjectPtr, idx: Py_ssize_t): PyObjectPtr;
    extern proc PyTuple_GetSlice(tup: PyObjectPtr, low: Py_ssize_t,
                                 high: Py_ssize_t): PyObjectPtr;
    extern proc PyTuple_SetItem(tup: PyObjectPtr, pos: Py_ssize_t,
                                item: PyObjectPtr);
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
    extern proc PyObject_CallMethod(obj: PyObjectPtr,
                                    method: c_ptrConst(c_char),
                                    format: c_ptrConst(c_char),
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
    extern proc PyBuffer_GetPointer(view: c_ptrConst(Py_buffer),
                                    indices: c_ptrConst(Py_ssize_t)): c_ptr(void);
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
    extern "chpl_PyBUF_STRIDES" const PyBUF_STRIDES: c_int;
    extern "chpl_PyBUF_C_CONTIGUOUS" const PyBUF_C_CONTIGUOUS: c_int;

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
                         size: Py_ssize_t,
                         ndim: Py_ssize_t,
                         shape: c_ptr(Py_ssize_t),
                         isOwned: bool): PyObjectPtr;

      var shape = arr.shape;
      var pyShape = allocate(Py_ssize_t, arr.rank.safeCast(c_size_t));
      for i in 0..# arr.rank {
        pyShape(i) = shape(i).safeCast(Py_ssize_t);
      }

      if isArrayType(T) {
        var sub = allocate(PyObjectPtr, arr.size.safeCast(c_size_t));
        for i in 0..#arr.size {
          sub(i) = createArray(arr(i));
        }
        return createPyArray(sub,
                             arr.size.safeCast(Py_ssize_t),
                             arr.rank.safeCast(Py_ssize_t),
                             pyShape,
                             true);
      } else {
        return createPyArray(c_ptrTo(arr),
                             arr.size.safeCast(Py_ssize_t),
                             arr.rank.safeCast(Py_ssize_t),
                             pyShape,
                             false);
      }

    }
  }

  /*
    A small subset of the pthread API
  */
  @chpldoc.nodoc
  module PThread {
    private use CTypes;

    extern type pthread_t;
    extern type pthread_attr_t;
    extern type pthread_mutex_t;
    extern type pthread_mutexattr_t;
    extern type pthread_cond_t;
    extern type pthread_condattr_t;
    extern proc pthread_create(thread: c_ptr(pthread_t),
                              attr: c_ptr(pthread_attr_t),
                              start_routine: c_fn_ptr,
                              arg: c_ptr(void)): c_int;

    extern proc pthread_join(thread: pthread_t,
                            retval: c_ptr(c_ptr(void))): c_int;
    extern proc pthread_mutex_init(mutex: c_ptr(pthread_mutex_t),
                                  attr: c_ptr(pthread_mutexattr_t)): c_int;
    extern proc pthread_mutex_destroy(mutex: c_ptr(pthread_mutex_t)): c_int;
    extern proc pthread_mutex_lock(mutex: c_ptr(pthread_mutex_t)): c_int;
    extern proc pthread_mutex_unlock(mutex: c_ptr(pthread_mutex_t)): c_int;
    extern proc pthread_cond_init(cond: c_ptr(pthread_cond_t),
                                  attr: c_ptr(pthread_condattr_t)): c_int;
    extern proc pthread_cond_destroy(cond: c_ptr(pthread_cond_t)): c_int;
    extern proc pthread_cond_wait(cond: c_ptr(pthread_cond_t),
                                  mutex: c_ptr(pthread_mutex_t)): c_int;
    extern proc pthread_cond_signal(cond: c_ptr(pthread_cond_t)): c_int;

    record pthreadSignal {
      var mutex: pthread_mutex_t;
      var cond: pthread_cond_t;
      var signaled: bool = false;
      proc init() {
        init this;
        pthread_mutex_init(c_ptrTo(mutex), nil);
        pthread_cond_init(c_ptrTo(cond), nil);
      }
      proc ref deinit() {
        pthread_mutex_destroy(c_ptrTo(mutex));
        pthread_cond_destroy(c_ptrTo(cond));
      }
      proc ref signal() {
        pthread_mutex_lock(c_ptrTo(mutex));
        signaled = true;
        pthread_cond_signal(c_ptrTo(cond));
        pthread_mutex_unlock(c_ptrTo(mutex));
      }
      proc ref wait() {
        pthread_mutex_lock(c_ptrTo(mutex));
        while !signaled {
          pthread_cond_wait(c_ptrTo(cond), c_ptrTo(mutex));
        }
        signaled = false;
        pthread_mutex_unlock(c_ptrTo(mutex));
      }
    }

  }

}
