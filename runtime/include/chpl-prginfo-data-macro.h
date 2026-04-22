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

#if !defined(CHPL_RT_PRGINFO_DATA_MACRO_DECLARE_ONLY_DONE) && \
     defined(CHPL_RT_PRGINFO_DATA_MACRO_DECLARE_ONLY)
  #define CHPL_RT_PRGINFO_DATA_MACRO_DECLARE_ONLY_DONE

  // These are the headers required to get at the types used below.
  //
  // NOTE: If you need to '#include' a type and that header ends up using
  // 'chpl-prginfo.h' it can cause some funky circular inclusion problems.
  // In that case, the best thing to do is to forward-declare the type
  // here if needed...
  #include <inttypes.h>
  #include <stddef.h>
  #include "chpl-string-support.h"
  #include "chpltypes.h"

  #ifndef LAUNCHER
    #include "qio_error.h"
  #else
    typedef void syserr;
  #endif

  // ...As is done here.
  struct qio_channel_s;
  typedef struct qio_channel_s qio_channel_t;

  // TODO: Could be removed if we had some formulaic way to pass a function
  //       type as a macro argument, but it doesn't seem like we can...?
  typedef void (*chpl_program_about_type)(void);
  typedef chpl_bool (*chpl_task_getCommDiagsTemporarilyDisabled_type)(void);
  typedef chpl_bool (*chpl_task_setCommDiagsTemporarilyDisabled_type)(chpl_bool);
  typedef void (*chpl_taskRunningCntInc_type)(int64_t ln, int64_t fn);
  typedef void (*chpl_taskRunningCntDec_type)(int64_t ln, int64_t fn);
  typedef void (*chpl_taskRunningCntReset_type)(int64_t ln, int64_t fn);
  typedef void (*CreateConfigVarTable_type)(void);
  typedef void (*chpl__initStringLiterals_type)(void);
  typedef void (*chpl__init_preInit_type)(int64_t ln, int32_t fn);
  typedef void (*chpl__init_PrintModuleInitOrder_type)(int64_t ln, int32_t fn);
  typedef void (*chpl__init_ChapelStandard_type)(int64_t ln, int32_t fn);
  typedef int64_t (*chpl_gen_main_type)(chpl_main_argument* _arg);
  typedef void (*chpl_memTracking_returnConfigVals_type)(chpl_bool* memTrack,
                                                         chpl_bool* memStats,
                                                         chpl_bool* memLeaksByType,
                                                         c_string* memLeaksByDesc,
                                                         chpl_bool* memLeaks,
                                                         size_t* memMax,
                                                         size_t* memThreshold,
                                                         c_string* memLog,
                                                         c_string* memLeaksLog);
  typedef syserr (*chpl_qio_fsync_type)(void* file);
  typedef syserr (*chpl_qio_getpath_type)(void* file, uint8_t** str, int64_t* len);
  typedef syserr (*chpl_qio_filelength_type)(void* file, int64_t* length);
  typedef syserr (*chpl_qio_setup_plugin_channel_type)(void* file, void** plugin_ch,
                                                       int64_t start,
                                                       int64_t end,
                                                       qio_channel_t* qio_ch);
  typedef syserr (*chpl_qio_channel_close_type)(void* ch);
  typedef syserr (*chpl_qio_read_atleast_type)(void* plugin_ch, int64_t amt);
  typedef syserr (*chpl_qio_write_type)(void* plugin_ch, int64_t amt);
  typedef syserr (*chpl_qio_get_chunk_type)(void* file, int64_t* length);
  typedef syserr (*chpl_qio_get_locales_for_region_type)(void* file, int64_t start,
                                                         int64_t end,
                                                         void **localeNamesPtr,
                                                         int64_t* nLocales);
  typedef syserr (*chpl_qio_file_close_type)(void* file);
  typedef c_sublocid_t
  (*chpl_localeModel_sublocToExecutionSubloc_type)(c_sublocid_t full_subloc);
  typedef void (*chpl__heapAllocateGlobals_type)(void);
  typedef int64_t (*chpl_mapPtrToIdxHere_type)(void* ptr, int64_t idx);
  typedef void* (*chpl_getPtrForIdxHere_type)(int64_t idx);
  typedef int (*chpl_areAnyChapelProgramsLoaded_type)(void);

  #define E_CALLBACK(name__)
  #define E_CONSTANT(name__, type__) typedef type__ name__##_type;
#endif

/**
  E_CONSTANT_RT(name, type)     | A constant used by the runtime.
  E_CALLBACK_RT(name)           | A callback used by the runtime.
  E_CONSTANT(name, type)        | Constant used by runtime or launcher.
  E_CALLBACK(name, type)        | Callback used by runtime or launcher.

  For callbacks, the type must be specified in the 'ifdef' block above using
  the format '"name"_type'. This is due to limitations of the preprocessor.
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
E_CALLBACK(chpl_program_about)

/** MODULE-CODE: ChapelTaskData.chpl
    Whether or not comm diagnostics is temporarily disabled.
*/
E_CALLBACK_RT(chpl_task_getCommDiagsTemporarilyDisabled)

/** MODULE-CODE: ChapelTaskData.chpl
    Whether or not comm diagnostics is temporarily disabled.
*/
E_CALLBACK_RT(chpl_task_setCommDiagsTemporarilyDisabled)

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
E_CALLBACK_RT(chpl_taskRunningCntInc)

/** MODULE-CODE: ChapelLocale.chpl
    Call to decrement the task running count. Seemingly only invoked
    by the compiler at codegen time (see 'gChplDecRunningTask').
*/
E_CALLBACK_RT(chpl_taskRunningCntDec)

/** MODULE-CODE: ChapelLocale.chpl
    Call to reset the task running count.
*/
E_CALLBACK_RT(chpl_taskRunningCntReset)

/** CODE-GENERATED
    Function which constructs the config variable table.
*/
E_CALLBACK(CreateConfigVarTable)

/** CODE-GENERATED
    Module initializer which contains all string literals.
*/
E_CALLBACK_RT(chpl__initStringLiterals)

/** CODE-GENERATED
    Module initializer which sets initialization flags to false.
    TODO: Why? Won't zero-initialization set these to false?
*/
E_CALLBACK_RT(chpl__init_preInit)

/** CODE-GENERATED
    Module initializer for the 'PrintModuleInitOrder' module.
*/
E_CALLBACK_RT(chpl__init_PrintModuleInitOrder)

/** CODE-GENERATED
    Module initializer for the 'ChapelStandard' module.
*/
E_CALLBACK_RT(chpl__init_ChapelStandard)

/** CODE-GENERATED
    Main entrypoint for this program.
*/
E_CALLBACK_RT(chpl_gen_main)

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

    // The compiler generates a separate array of descriptions for the
    // allocation types it defines. Indices into that compiler-generated
    // array conceptually start after the CHPL_RT_MD_NUM enum value in
    // chpl-mem.h).  This is that compiler-generated array, and how many
    // entries it has (also defined in the generated code).
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
E_CALLBACK_RT(chpl_memTracking_returnConfigVals)

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
    Location of LLVM binary directory (TODO: I don't see where this is set?).
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
    // These functions are exported from the locale model for use by
    // the tasking layer to convert between a full sublocale and an
    // execution sublocale.
*/
E_CALLBACK_RT(chpl_localeModel_sublocToExecutionSubloc)

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
E_CALLBACK_RT(chpl__heapAllocateGlobals)

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
E_CALLBACK_RT(chpl_qio_setup_plugin_channel)

/** MODULE-CODE: IO.chpl
    Reads 'amt' bytes (or more) into the channel buffer.
*/
E_CALLBACK_RT(chpl_qio_read_atleast)

/** MODULE-CODE: IO.chpl
    Writes 'amt' bytes from the channel buffer.
*/
E_CALLBACK_RT(chpl_qio_write)

/** MODULE-CODE: IO.chpl
    Close the channel.
*/
E_CALLBACK_RT(chpl_qio_channel_close)

/** MODULE-CODE: IO.chpl
    Get the length of a file.
*/
E_CALLBACK_RT(chpl_qio_filelength)

/** MODULE-CODE: IO.chpl
    Get the path to a file.
*/
E_CALLBACK_RT(chpl_qio_getpath)

/** MODULE-CODE: IO.chpl
    Performs a 'fsync' operation.
*/
E_CALLBACK_RT(chpl_qio_fsync)

/** MODULE-CODE: IO.chpl
    Get the optimal I/O size for the channel.
*/
E_CALLBACK_RT(chpl_qio_get_chunk)

/** MODULE-CODE: IO.chpl
    Get the locales for a region. The 'localeNamesPtr' should be a pointer
    to an array of 'char*' to set on output.
*/
E_CALLBACK_RT(chpl_qio_get_locales_for_region)

/** MODULE-CODE: IO.chpl
    Close a file.
*/
E_CALLBACK_RT(chpl_qio_file_close)

/** MODULE-CODE: ChapelDynamicLoading.chpl
    Map a pointer to an index in the pointer cache on this locale.
*/
E_CALLBACK_RT(chpl_mapPtrToIdxHere)

/** MODULE-CODE: ChapelDynamicLoading.chpl
    Get a pointer for an index in the pointer cache on this locale.
*/
E_CALLBACK_RT(chpl_getPtrForIdxHere)

/** MODULE-CODE: ChapelDynamicLoading.chpl
    Check to see if any Chapel programs are currently loaded.
*/
E_CALLBACK_RT(chpl_areAnyChapelProgramsLoaded)
