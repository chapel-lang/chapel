/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

module ChapelProgramEntrypoints {
  use ChapelBase;
  use ChapelProgramRegistration;
  use ChapelRuntimeInterface;
  use CTypes;

  // This is an opaque alias for "char" used to make sure the compiler will
  // generate the proper type instead of e.g., "int(8)" as is usually done
  // for "c_char". Once inside a function passed this type, you can use a
  // helper to cast back to "c_char".
  pragma "opaque c type alias"
  extern "char" type chpl_opaque_c_char;

  pragma "opaque c type alias"
  extern "int" type chpl_opaque_c_int;

  // Alias for 'char**' to be used as the type of 'argv'.
  type chpl_opaque_argv_array = c_ptr(c_ptr(chpl_opaque_c_char));

  pragma "locale private"
  var chpl_isLibInitialized = false;

  pragma "locale private"
  var chpl_isLibFinalized = false;

  private inline proc rtError(msg: c_ptrConst(c_char)) {
    extern proc chpl_error(message: c_ptrConst(c_char),
                           lineno: int(32),
                           filenameIdx: int(32)): void;
    chpl_error(msg, 0, 0);
  }

  pragma "locale private"   // TODO: May not need this, but...
  pragma "no init"          // Don't overwite work done later.
  private var chpl_genMainArg: chpl_main_argument;

  // For the runtime. Get a pointer to the main argument on this locale.
  export proc chpl_genMainArgPtr: c_ptr(chpl_main_argument) {
    return c_ptrTo(chpl_genMainArg);
  }

  private proc initTaskDynamicEndCount() {
    var endCount = _endCountAlloc(forceLocalTypes=false);
    chpl_task_setDynamicEndCount(endCount);
  }

  private proc destroyTaskDynamicEndCount() {
    var endCount = chpl_task_getDynamicEndCount();
    _waitEndCount(endCount);
    _endCountFree(endCount);
  }

  //
  // A program using Chapel as a library might look like:
  //
  // int main(int argc, char* argv) {
  //
  //   chpl_library_init(...)
  //   chpl__init_MODULE_1(LINE, FILENAME)
  //   ...
  //   chpl__init_MODULE_N(LINE, FILENAME)
  //
  //   call_chapel_function_from_MODULE_1()
  //   ...
  //   call_chapel_function_from_MODULE_N()
  //
  //   chpl_library_finalize()
  //
  // }
  //
  // TODO (dlongnecke): This function needs to be documented as one that also
  //                    initializes the runtime, or we need to make sure that
  //                    runtime initialization is split out from library init.
  export proc chpl_library_init(argc: chpl_opaque_c_int,
                                argv: chpl_opaque_argv_array) {
    extern proc chpl_rt_init(root_prg: c_ptr(chpl_rt_prginfo),
                             argc: chpl_opaque_c_int,
                             argv: chpl_opaque_argv_array): void;
    // TODO: A lie, 'chpl_main' is actually a local function pointer.
    extern proc chpl_task_callMain(chpl_main: c_ptr(void)): void;
    extern proc chpl_libraryModuleLevelSetup(): void;

    if chpl_isLibInitialized {
      // Ok to emit message as runtime is already set up.
      rtError("Can't call chpl_library_init() twice");
    }

    if numLocales > 1 then {
      rtError("Should not invoke when running with more than one locale");
      return;
    }

    const ptr = chpl_prepareProgramInfoHere();

    // NOTE: The call 'chpl_rt_init' is idempotent and nothing will happen
    // if the runtime is already initialized.
    chpl_rt_init(ptr, argc, argv);

    // NOTE: This init function is a shim defined in 'ChapelRuntimeInterface'.
    // TODO: There really needs to be a better way to do this. Is the normal
    //       casting not working because the proc-ptr is a class right now?
    //       How am I supposed to trigger a local pointer to be created again?
    const p1: c_fn_ptr = __primitive("capture fn",
                                     chpl_initProgramStandardModules,
                                     true);
    const p2 = p1 : c_ptr(void);

    chpl_task_callMain(p2);

    // @dlongnecke-cray, 11/16/2020
    // TODO: Call chpl_preUserCodeSync() here for Locale[0]?
    initTaskDynamicEndCount();

    // Now that module initialization is done, set the flag to 'true'.
    chpl_isLibInitialized = true;
  }

  export proc chpl_library_finalize() {
    extern proc chpl_libraryModuleLevelSetup(): void;
    extern proc chpl_deinitModules(): void;
    extern proc chpl_finalize(status: c_int, all: c_int): void;

    if !chpl_isLibInitialized || chpl_isLibFinalized then return;

    destroyTaskDynamicEndCount();
    chpl_deinitModules();
    chpl_finalize(0, 1);

    // Now that finalization is done, set the flag to 'true'.
    chpl_isLibFinalized = true;
  }

  /** This is invoked by loaded Chapel programs to set them up. */
  export proc chpl_initLoadedProgramModulesHere() {

    // This function is generated by the compiler.
    extern proc chpl_initProgramCommandLineModules(): void;

    // This is a runtime shim, the underlying runtime function does hybrid
    // initialization: it initializes some important modules on all locales,
    // and then it performs some L0-only initialization.
    chpl_initProgramStandardModules();

    if chpl_nodeID == 0 {
      // At this point everything is initialized, so now we only run on L0.
      // The other locales are checked in and waiting, so L0 must also check
      // in to complete the barrier. This step is identical to some of the
      // setup steps that occur in 'chpl_gen_main'.
      chpl_preUserCodeSync();

      // Now we initialize user modules. Execution begins only on L0.
      //
      // TODO: There is more code in 'chpl_gen_main' e.g., code to initialize
      //       the dynamic end count for the main task. Do we need to do that
      //       here, or is it OK because P1 is loaded and things are already
      //       initialized?
      chpl_initProgramCommandLineModules();
    }
  }
}
