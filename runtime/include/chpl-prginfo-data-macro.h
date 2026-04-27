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

/**
  E_CONSTANT_RT(name, type)                 | A constant used by the runtime.
  E_CALLBACK_RT(name, ret_type, formals)    | A callback used by the runtime.
  E_CONSTANT(name, type)                    | Used by runtime or launcher.
  E_CALLBACK(name, ret_type, formals)       | Used by runtime or launcher.
*/

/** CODE-GENERATED
    Whether or not 'main' can accept arguments.
*/
E_CONSTANT(mainHasArgs, int)

/** CODE-GENERATED
    Whether or not 'main' should preserve the delimiter when parsing args.
*/
E_CONSTANT(mainPreserveDelimiter, int)

/** CODE-GENERATED
    Provides information about this program.
*/
E_CALLBACK(chpl_program_about, void, void)

/** MODULE-CODE: ChapelTaskData.chpl
    Get whether or not comm diagnostics is temporarily disabled.
*/
E_CALLBACK_RT(chpl_task_getCommDiagsTemporarilyDisabled, chpl_bool, void)

/** MODULE-CODE: ChapelTaskData.chpl
    Set whether or not comm diagnostics is temporarily disabled.
*/
E_CALLBACK_RT(chpl_task_setCommDiagsTemporarilyDisabled, void, chpl_bool)

/** CODE-GENERATED
    Table of private broadcast constants.
*/
E_CONSTANT_RT(chpl_private_broadcast_table, void**)

/** CODE-GENERATED
    Length of table of private broadcast constants.
*/
E_CONSTANT_RT(chpl_private_broadcast_table_len, int)

/** CODE-GENERATED
    TODO: Not exactly sure what this thing is. Table of serializers for RVF?
*/
E_CONSTANT_RT(chpl_global_serialize_table, void**)

/** CODE-GENERATED
    A table of local addresses of wide pointers containing global vars.
*/
E_CONSTANT_RT(chpl_globals_registry, wide_ptr_t**)

/** CODE-GENERATED
    The number of globals that live on the heap.
*/
E_CONSTANT_RT(chpl_numGlobalsOnHeap, int)

/** CODE-GENERATED
    The function table.
*/
E_CONSTANT_RT(chpl_ftable, chpl_fn_p*)

/** CODE-GENERATED (TODO NOT USED)
    Length of the function table.
*/
E_CONSTANT_RT(chpl_ftableSize, int64_t)

/** MODULE-CODE: ChapelLocale.chpl
    Call to increment the task running count.
*/
E_CALLBACK_RT(chpl_taskRunningCntInc, void, int32_t ln, int32_t fn)

/** MODULE-CODE: ChapelLocale.chpl
    Call to decrement the task running count. Seemingly only invoked
    by the compiler at codegen time (see 'gChplDecRunningTask').
*/
E_CALLBACK_RT(chpl_taskRunningCntDec, void, int32_t ln, int32_t fn)

/** MODULE-CODE: ChapelLocale.chpl
    Call to reset the task running count.
*/
E_CALLBACK_RT(chpl_taskRunningCntReset, void, int32_t ln, int32_t fn)

/** CODE-GENERATED
    Function which constructs the config variable table.
*/
E_CALLBACK(CreateConfigVarTable, void, void)

/** CODE-GENERATED
    Module initializer which contains all string literals.
*/
E_CALLBACK_RT(chpl__initStringLiterals, void, void)

/** CODE-GENERATED
    Module initializer which sets initialization flags to false.
    TODO: Why? Won't zero-initialization set these to false?
*/
E_CALLBACK_RT(chpl__init_preInit, void, int32_t ln, int32_t fn)

/** CODE-GENERATED
    Module initializer for the 'PrintModuleInitOrder' module.
*/
E_CALLBACK_RT(chpl__init_PrintModuleInitOrder, void, int32_t ln, int32_t fn)

/** CODE-GENERATED
    Module initializer for the 'ChapelStandard' module.
*/
E_CALLBACK_RT(chpl__init_ChapelStandard, void, int32_t ln, int32_t fn)

/** CODE-GENERATED
    Main entrypoint for this program.
*/
E_CALLBACK_RT(chpl_gen_main, int64_t, chpl_main_argument* _arg)

/** CODE-GENERATED
    Contains filenames, and maybe other strings besides?
*/
E_CONSTANT(chpl_filenameTable, c_string*)

/** CODE-GENERATED
    Length of the filename table.
*/
E_CONSTANT(chpl_filenameTableSize, int32_t)

/** CODE-GENERATED
    Contains strings that describe allocation types.

    The compiler generates a separate array of descriptions for the
    allocation types it defines. Indices into that compiler-generated
    array conceptually start after the CHPL_RT_MD_NUM enum value in
    chpl-mem.h).  This is that compiler-generated array, and how many
    entries it has (also defined in the generated code).
*/
E_CONSTANT_RT(chpl_mem_descs, const char**)

/** CODE-GENERATED
    Length of the memory description table.
*/
E_CONSTANT_RT(chpl_mem_numDescs, int)

/** MODULE-CODE: MemTracking.chpl
    This is a giant callback which writes the config vars used to control
    memory tracking to runtime memory.
*/
E_CALLBACK_RT(chpl_memTracking_returnConfigVals, void,
              chpl_bool* memTrack,
              chpl_bool* memStats,
              chpl_bool* memLeaksByType,
              c_string* memLeaksByDesc,
              chpl_bool* memLeaks,
              size_t* memMax,
              size_t* memThreshold,
              c_string* memLog,
              c_string* memLeaksLog)

/** CODE-GENERATED
    Text of the compiler invocation used to create this program.
*/
E_CONSTANT_RT(chpl_compileCommand, const char*)

/** CODE-GENERATED
    Directory where the compiler invocation took place.
*/
E_CONSTANT_RT(chpl_compileDirectory, const char*)

/** CODE-GENERATED
    Directory to save temporary compilation artifacts.
*/
E_CONSTANT_RT(chpl_saveCDir, const char*)

/** CODE-GENERATED
    Location of '$CHPL_HOME' when this was compiled.
*/
E_CONSTANT_RT(CHPL_HOME, const char*)

/** CODE-GENERATED
    If '--cache-remote' was set when this program was compiled.
*/
E_CONSTANT_RT(CHPL_CACHE_REMOTE, int)

/** CODE-GENERATED
    If '--warn-unstable' was set when this program was compiled.
*/
E_CONSTANT(warnUnstable, int)

/** CODE-GENERATED
    Location of LLVM binary directory when this program was compiled.
*/
E_CONSTANT(CHPL_LLVM_BIN_DIR, const char*)

/** CODE-GENERATED
    One piece of the symbol table. This is a flat 1D array with 2 elements
    per entry, so you need to stride indices by (idx*2). This portion of
    the table contains two 'c_int' per entry: (Chapel file name index,
    Chapel line number). TODO: Could we just refactor this whole table
    to be defined in the runtime and use a 'struct' instead for clarity?
*/
E_CONSTANT_RT(chpl_filenumSymTable, int*)

/** CODE-GENERATED
    One piece of the symbol table. This is a flat 1D array with 2 elements
    per entry, so you need to scale indices by (idx*2). This portion of
    the table contains two 'const char*' per entry: (C name, Chapel name).
*/
E_CONSTANT_RT(chpl_funSymTable, const char**)

/** CODE-GENERATED
    The number of entries in the combined 'Chapel symbol table'.
*/
E_CONSTANT_RT(chpl_sizeSymTable, int32_t)

/** CODE-GENERATED
    Value of '$CHPL_UNWIND' when this program was compiled.
*/
E_CONSTANT_RT(CHPL_UNWIND, const char*)

/** CODE-GENERATED
    If '--interleave-memory' was set when this program was compiled.
*/
E_CONSTANT_RT(CHPL_INTERLEAVE_MEM, int)

/** MODULE-CODE: LocaleModel.chpl
    Convert from a full sublocale to an execution sublocale.

    These functions are exported from the locale model for use by
    the tasking layer to convert between a full sublocale and an
    execution sublocale.
*/
E_CALLBACK_RT(chpl_localeModel_sublocToExecutionSubloc, c_sublocid_t,
              c_sublocid_t full_subloc)

/** CODE-GENERATED
    Value of '$CHPL_COMM' when this program was compiled.
*/
E_CONSTANT_RT(CHPL_COMM, const char*)

/** CODE-GENERATED
    If '--stack-checks' was set when this program was compiled.
*/
E_CONSTANT_RT(CHPL_STACK_CHECKS, int)

/** CODE-GENERATED
    Value of '$CHPL_TARGET_PLATFORM' when this program was compiled.
*/
E_CONSTANT_RT(CHPL_TARGET_PLATFORM, const char*)

/** CODE-GENERATED
    Value of '$CHPL_TARGET_MEM' when this program was compiled.
    TODO: Could be dropped since it's only used by 'COMM=ugni'.
*/
E_CONSTANT_RT(CHPL_TARGET_MEM, const char*)

/** CODE-GENERATED
    Call to heap allocate all global variables.
*/
E_CALLBACK_RT(chpl__heapAllocateGlobals, void, void)

/** CODE-GENERATED
    Table containing function info.
*/
E_CONSTANT_RT(chpl_finfo, const chpl_fn_info*)

/** CODE-GENERATED
    Value of '$CHPL_LOCALE_MODEL' when this program was compiled.
*/
E_CONSTANT_RT(CHPL_LOCALE_MODEL, const char*)

/** CODE-GENERATED
    Value of '$CHPL_TARGET_CPU' when this program was compiled.
*/
E_CONSTANT_RT(CHPL_TARGET_CPU, const char*)

/** CODE-GENERATED
    Value of '$CHPL_GASNET_SEGMENT' when this program was compiled.
*/
E_CONSTANT_RT(CHPL_GASNET_SEGMENT, const char*)

/** MODULE-CODE: IO.chpl
    Create a plugin channel to attach to the qio channel.
*/
E_CALLBACK_RT(chpl_qio_setup_plugin_channel, syserr,
              void* file, void** plugin_ch,
              int64_t start,
              int64_t end,
              qio_channel_t* qio_ch)

/** MODULE-CODE: IO.chpl
    Reads 'amt' bytes (or more) into the channel buffer.
*/
E_CALLBACK_RT(chpl_qio_read_atleast, syserr, void* plugin_ch, int64_t amt)

/** MODULE-CODE: IO.chpl
    Writes 'amt' bytes from the channel buffer.
*/
E_CALLBACK_RT(chpl_qio_write, syserr, void* plugin_ch, int64_t amt)

/** MODULE-CODE: IO.chpl
    Close the channel.
*/
E_CALLBACK_RT(chpl_qio_channel_close, syserr, void* file)

/** MODULE-CODE: IO.chpl
    Get the length of a file.
*/
E_CALLBACK_RT(chpl_qio_filelength, syserr, void* file, int64_t* length)

/** MODULE-CODE: IO.chpl
    Get the path to a file.
*/
E_CALLBACK_RT(chpl_qio_getpath, syserr,
              void* file,
              uint8_t** str,
              int64_t* len)

/** MODULE-CODE: IO.chpl
    Performs a 'fsync' operation.
*/
E_CALLBACK_RT(chpl_qio_fsync, syserr, void* file)

/** MODULE-CODE: IO.chpl
    Get the optimal I/O size for the channel.
*/
E_CALLBACK_RT(chpl_qio_get_chunk, syserr, void* file, int64_t* length)

/** MODULE-CODE: IO.chpl
    Get the locales for a region. The 'localeNamesPtr' should be a pointer
    to an array of 'char*' to set on output.
*/
E_CALLBACK_RT(chpl_qio_get_locales_for_region, syserr,
              void* file, int64_t start, int64_t end,
              void **localeNamesPtr,
              int64_t* nLocales)

/** MODULE-CODE: IO.chpl
    Close a file.
*/
E_CALLBACK_RT(chpl_qio_file_close, syserr, void* ch)

/** MODULE-CODE: ChapelDynamicLoading.chpl
    Map a pointer to an index in the pointer cache on this locale.
*/
E_CALLBACK_RT(chpl_mapPtrToIdxHere, int64_t, void* ptr, int64_t idx)

/** MODULE-CODE: ChapelDynamicLoading.chpl
    Get a pointer for an index in the pointer cache on this locale.
*/
E_CALLBACK_RT(chpl_getPtrForIdxHere, void*, int64_t idx)

/** MODULE-CODE: ChapelDynamicLoading.chpl
    Check to see if any Chapel programs are currently loaded.
*/
E_CALLBACK_RT(chpl_areAnyChapelProgramsLoaded, int, void)
