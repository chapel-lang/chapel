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

/*
Access to configuration information for the 'chpl' compiler.

This module's contents provide access to compile-time aspects of
Chapel's configuration, such as those specified by ``CHPL_*``
environment variables or ``chpl`` command-line options, as well as
those inferred from the environment or other settings.  For details,
refer to :ref:`readme-chplenv` and the :ref:`chpl man page
<man-chpl>`.  This information can also be displayed from a Chapel
executable's command line by executing it with the ``--about`` flag.
*/

module ChplConfig {
  private use String;
  private use ChapelStandard;

  /* See :ref:`readme-chplenv.CHPL_HOME` for more information. */
  @unstable("'ChplConfig.CHPL_HOME' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_HOME:string;
  CHPL_HOME = __primitive("get compiler variable", "CHPL_HOME");

  /* See :ref:`readme-chplenv.CHPL_TARGET_PLATFORM` for more information. */
  @unstable("'ChplConfig.CHPL_TARGET_PLATFORM' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_TARGET_PLATFORM:string;
  CHPL_TARGET_PLATFORM = __primitive("get compiler variable", "CHPL_TARGET_PLATFORM");

  /* See :ref:`readme-chplenv.CHPL_HOST_PLATFORM` for more information. */
  @unstable("'ChplConfig.CHPL_HOST_PLATFORM' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_HOST_PLATFORM:string;
  CHPL_HOST_PLATFORM = __primitive("get compiler variable", "CHPL_HOST_PLATFORM");

  /* See :ref:`readme-chplenv.CHPL_HOST_ARCH` for more information. */
  @unstable("'ChplConfig.CHPL_HOST_ARCH' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_HOST_ARCH:string;
  CHPL_HOST_ARCH = __primitive("get compiler variable", "CHPL_HOST_ARCH");

  /* See :ref:`readme-chplenv.CHPL_COMPILER` for more information. */
  @unstable("'ChplConfig.CHPL_HOST_COMPILER' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_HOST_COMPILER:string;
  CHPL_HOST_COMPILER = __primitive("get compiler variable", "CHPL_HOST_COMPILER");

  /* See :ref:`readme-chplenv.CHPL_COMPILER` for more information. */
  @unstable("'ChplConfig.CHPL_TARGET_COMPILER' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_TARGET_COMPILER:string;
  CHPL_TARGET_COMPILER = __primitive("get compiler variable", "CHPL_TARGET_COMPILER");

  /* See :ref:`readme-chplenv.CHPL_TARGET_ARCH` for more information. */
  @unstable("'ChplConfig.CHPL_TARGET_ARCH' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_TARGET_ARCH:string;
  CHPL_TARGET_ARCH = __primitive("get compiler variable", "CHPL_TARGET_ARCH");

  /* See :ref:`readme-chplenv.CHPL_TARGET_CPU` for more information. */
  @unstable("'ChplConfig.CHPL_TARGET_CPU' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_TARGET_CPU:string;
  CHPL_TARGET_CPU = __primitive("get compiler variable", "CHPL_TARGET_CPU");

  /* See :ref:`readme-chplenv.CHPL_LOCALE_MODEL` for more information. */
  @unstable("'ChplConfig.CHPL_LOCALE_MODEL' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_LOCALE_MODEL:string;
  CHPL_LOCALE_MODEL = __primitive("get compiler variable", "CHPL_LOCALE_MODEL");

  /* Check if the Chapel program is being compiled for a single locale only. */
  proc compiledForSingleLocale() param do return _local;

  /* See :ref:`readme-chplenv.CHPL_COMM` for more information. */
  @unstable("'ChplConfig.CHPL_COMM' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_COMM:string;
  CHPL_COMM = __primitive("get compiler variable", "CHPL_COMM");

  /* See :ref:`readme-launcher` for more information. */
  @unstable("'ChplConfig.CHPL_COMM_SUBSTRATE' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_COMM_SUBSTRATE:string;
  CHPL_COMM_SUBSTRATE = __primitive("get compiler variable", "CHPL_COMM_SUBSTRATE");

  /* See :ref:`readme-multilocale` for more information. */
  @unstable("'ChplConfig.CHPL_GASNET_SEGMENT' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_GASNET_SEGMENT:string;
  CHPL_GASNET_SEGMENT = __primitive("get compiler variable", "CHPL_GASNET_SEGMENT");

  @chpldoc.nodoc
  @unstable("'ChplConfig.CHPL_LIBFABRIC' is unstable and may be replaced with a different way to access this information in the future")
  /* See :ref:`readme-libfabric` for more information. */
  param CHPL_LIBFABRIC:string;
  CHPL_LIBFABRIC = __primitive("get compiler variable", "CHPL_LIBFABRIC");

  @chpldoc.nodoc
  @unstable("'ChplConfig.CHPL_COMM_OFI_OOB' is unstable and may be replaced with a different way to access this information in the future")
  /* See :ref:`readme-libfabric` for more information. */
  param CHPL_COMM_OFI_OOB:string;
  CHPL_COMM_OFI_OOB = __primitive("get compiler variable", "CHPL_COMM_OFI_OOB");

  /* See :ref:`readme-chplenv.CHPL_TASKS` for more information. */
  @unstable("'ChplConfig.CHPL_TASKS' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_TASKS:string;
  CHPL_TASKS = __primitive("get compiler variable", "CHPL_TASKS");

  /* See :ref:`readme-chplenv.CHPL_LAUNCHER` for more information. */
  @unstable("'ChplConfig.CHPL_LAUNCHER' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_LAUNCHER:string;
  CHPL_LAUNCHER = __primitive("get compiler variable", "CHPL_LAUNCHER");

  /* See :ref:`readme-chplenv.CHPL_TIMERS` for more information. */
  @unstable("'ChplConfig.CHPL_TIMERS' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_TIMERS:string;
  CHPL_TIMERS = __primitive("get compiler variable", "CHPL_TIMERS");

  /* See :ref:`readme-chplenv.CHPL_UNWIND` for more information. */
  @unstable("'ChplConfig.CHPL_UNWIND' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_UNWIND:string;
  CHPL_UNWIND = __primitive("get compiler variable", "CHPL_UNWIND");

  /* See :ref:`readme-chplenv.CHPL_MEM` for more information. */
  @unstable("'ChplConfig.CHPL_MEM' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_MEM:string;
  CHPL_MEM = __primitive("get compiler variable", "CHPL_MEM");

  /* See :ref:`readme-chplenv.CHPL_MAKE` for more information. */
  @unstable("'ChplConfig.CHPL_MAKE' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_MAKE:string;
  CHPL_MAKE = __primitive("get compiler variable", "CHPL_MAKE");

  /* See :ref:`readme-chplenv.CHPL_ATOMICS` for more information. */
  @unstable("'ChplConfig.CHPL_ATOMICS' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_ATOMICS:string;
  CHPL_ATOMICS = __primitive("get compiler variable", "CHPL_ATOMICS");

  /* See :ref:`readme-atomics` for more information. */
  @unstable("'ChplConfig.CHPL_NETWORK_ATOMICS' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_NETWORK_ATOMICS:string;
  CHPL_NETWORK_ATOMICS = __primitive("get compiler variable", "CHPL_NETWORK_ATOMICS");

  /* See :ref:`readme-chplenv.CHPL_GMP` for more information. */
  @unstable("'ChplConfig.CHPL_GMP' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_GMP:string;
  CHPL_GMP = __primitive("get compiler variable", "CHPL_GMP");

  /* See :ref:`readme-chplenv.CHPL_HWLOC` for more information. */
  @unstable("'ChplConfig.CHPL_HWLOC' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_HWLOC:string;
  CHPL_HWLOC = __primitive("get compiler variable", "CHPL_HWLOC");

  @chpldoc.nodoc
  @unstable("'ChplConfig.CHPL_JEMALLOC' is unstable and may be replaced with a different way to access this information in the future")
  /* See :ref:`readme-chplenv.CHPL_JEMALLOC` for more information. */
  param CHPL_JEMALLOC:string;
  CHPL_JEMALLOC = __primitive("get compiler variable", "CHPL_TARGET_JEMALLOC");

  /* See :ref:`readme-chplenv.CHPL_RE2` for more information. */
  @unstable("'ChplConfig.CHPL_RE2' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_RE2:string;
  CHPL_RE2 = __primitive("get compiler variable", "CHPL_RE2");

  /* See :ref:`readme-chplenv.CHPL_LLVM` for more information. */
  @unstable("'ChplConfig.CHPL_LLVM' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_LLVM:string;
  CHPL_LLVM = __primitive("get compiler variable", "CHPL_LLVM");

  @chpldoc.nodoc
  @unstable("'ChplConfig.CHPL_GPU_MEM_STRATEGY' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_GPU_MEM_STRATEGY:string;
  CHPL_GPU_MEM_STRATEGY = __primitive("get compiler variable", "CHPL_GPU_MEM_STRATEGY");

  @chpldoc.nodoc
  @unstable("'ChplConfig.CHPL_GPU' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_GPU:string;
  CHPL_GPU = __primitive("get compiler variable", "CHPL_GPU");

  @chpldoc.nodoc
  @unstable("'ChplConfig.CHPL_LIB_PIC' is unstable and may be replaced with a different way to access this information in the future")
  param CHPL_LIB_PIC: string;
  CHPL_LIB_PIC = __primitive("get compiler variable", "CHPL_LIB_PIC");
}
