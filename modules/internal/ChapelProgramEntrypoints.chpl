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
  use ChapelBase, CTypes;

  // Alias for 'char**' to be used as the type of 'argv'.
  type c_argArray = c_ptr(c_ptr(c_char));

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
  export proc chpl_library_init(argc: c_int, argv: c_argArray) {
    extern proc chpl_rt_init(argc: c_int, argv: c_argArray): void;
    // TODO: A lie, 'chpl_main' is actually a local function pointer.
    extern proc chpl_task_callMain(chpl_main: c_ptr(void)): void;
    extern proc chpl_std_module_init(): void;
    extern proc chpl_libraryModuleLevelSetup(): void;

    if chpl_isLibInitialized {
      // Ok to emit message as runtime is already set up.
      rtError("Can't call chpl_library_init() twice");

    }

    if numLocales > 1 then {
      rtError("Should not invoke when running with more than one locale");
      return;
    }

    // NOTE: The call 'chpl_rt_init' is idempotent and nothing will happen
    // if the runtime is already initialized.
    chpl_rt_init(argc, argv);

    // TODO: There really needs to be a better way to do this. Is the normal
    //       casting not working because the proc-ptr is a class right now?
    //       How am I supposed to trigger a local pointer to be created again?
    const p1: c_fn_ptr = __primitive("capture fn", chpl_std_module_init, true);
    const p2 = p1 : c_ptr(void);

    chpl_task_callMain(p2);

    // @dlongnecke-cray, 11/16/2020
    // TODO: Call chpl_rt_preUserCodeHook() here for Locale[0]?
    chpl_libraryModuleLevelSetup();

    // Now that module initialization is done, set the flag to 'true'.
    chpl_isLibInitialized = true;
  }

  export proc chpl_library_finalize() {
    extern proc chpl_libraryModuleLevelSetup(): void;
    extern proc chpl_deinitModules(): void;
    extern proc chpl_finalize(status: c_int, all: c_int): void;

    if !chpl_isLibInitialized || chpl_isLibFinalized then return;

    chpl_libraryModuleLevelCleanup();
    chpl_deinitModules();
    chpl_finalize(0, 1);

    // Now that finalization is done, set the flag to 'true'.
    chpl_isLibFinalized = true;
  }
}
