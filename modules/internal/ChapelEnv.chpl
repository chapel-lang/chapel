/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

//
// ChapelEnv.chpl
//

/*

    The values of Chapel's environment variables upon compile time are
    accessible through the built-in parameters shown below. This information
    can also be displayed from the command line by executing the compiled
    program with the ``--about`` flag.

 */
module ChapelEnv {
  private use String;
  private use ChapelStandard;

  /* See :ref:`readme-chplenv.CHPL_HOME` for more information. */
  param CHPL_HOME:string            = __primitive("get compiler variable", "CHPL_HOME");

  /* Deprecated */
  param CHPL_AUX_FILESYS:string     = __primitive("get compiler variable", "CHPL_AUX_FILESYS");

  /* See :ref:`readme-chplenv.CHPL_TARGET_PLATFORM` for more information. */
  param CHPL_TARGET_PLATFORM:string = __primitive("get compiler variable", "CHPL_TARGET_PLATFORM");

  /* See :ref:`readme-chplenv.CHPL_HOST_PLATFORM` for more information. */
  param CHPL_HOST_PLATFORM:string   = __primitive("get compiler variable", "CHPL_HOST_PLATFORM");

  /* See :ref:`readme-chplenv.CHPL_HOST_ARCH` for more information. */
  param CHPL_HOST_ARCH:string   = __primitive("get compiler variable", "CHPL_HOST_ARCH");

  /* See :ref:`readme-chplenv.CHPL_COMPILER` for more information. */
  param CHPL_HOST_COMPILER:string   = __primitive("get compiler variable", "CHPL_HOST_COMPILER");

  /* See :ref:`readme-chplenv.CHPL_COMPILER` for more information. */
  param CHPL_TARGET_COMPILER:string = __primitive("get compiler variable", "CHPL_TARGET_COMPILER");

  /* See :ref:`readme-chplenv.CHPL_TARGET_ARCH` for more information. */
  param CHPL_TARGET_ARCH:string     = __primitive("get compiler variable", "CHPL_TARGET_ARCH");

  /* See :ref:`readme-chplenv.CHPL_TARGET_CPU` for more information. */
  param CHPL_TARGET_CPU:string     = __primitive("get compiler variable", "CHPL_TARGET_CPU");

  /* See :ref:`readme-chplenv.CHPL_LOCALE_MODEL` for more information. */
  param CHPL_LOCALE_MODEL:string    = __primitive("get compiler variable", "CHPL_LOCALE_MODEL");

  /* See :ref:`readme-chplenv.CHPL_COMM` for more information. */
  param CHPL_COMM:string            = __primitive("get compiler variable", "CHPL_COMM");

  /* See :ref:`readme-launcher` for more information. */
  param CHPL_COMM_SUBSTRATE:string  = __primitive("get compiler variable", "CHPL_COMM_SUBSTRATE");

  /* See :ref:`readme-multilocale` for more information. */
  param CHPL_GASNET_SEGMENT:string  = __primitive("get compiler variable", "CHPL_GASNET_SEGMENT");

  /* See :ref:`readme-chplenv.CHPL_TASKS` for more information. */
  param CHPL_TASKS:string           = __primitive("get compiler variable", "CHPL_TASKS");

  /* See :ref:`readme-chplenv.CHPL_LAUNCHER` for more information. */
  param CHPL_LAUNCHER:string        = __primitive("get compiler variable", "CHPL_LAUNCHER");

  /* See :ref:`readme-chplenv.CHPL_TIMERS` for more information. */
  param CHPL_TIMERS:string          = __primitive("get compiler variable", "CHPL_TIMERS");

  /* See :ref:`readme-chplenv.CHPL_UNWIND` for more information. */
  param CHPL_UNWIND:string          = __primitive("get compiler variable", "CHPL_UNWIND");

  /* See :ref:`readme-chplenv.CHPL_MEM` for more information. */
  param CHPL_MEM:string             = __primitive("get compiler variable", "CHPL_MEM");

  /* See :ref:`readme-chplenv.CHPL_MAKE` for more information. */
  param CHPL_MAKE:string            = __primitive("get compiler variable", "CHPL_MAKE");

  /* See :ref:`readme-chplenv.CHPL_ATOMICS` for more information. */
  param CHPL_ATOMICS:string         = __primitive("get compiler variable", "CHPL_ATOMICS");

  /* See :ref:`readme-atomics` for more information. */
  param CHPL_NETWORK_ATOMICS:string = __primitive("get compiler variable", "CHPL_NETWORK_ATOMICS");

  /* See :ref:`readme-chplenv.CHPL_GMP` for more information. */
  param CHPL_GMP:string             = __primitive("get compiler variable", "CHPL_GMP");

  /* See :ref:`readme-chplenv.CHPL_HWLOC` for more information. */
  param CHPL_HWLOC:string           = __primitive("get compiler variable", "CHPL_HWLOC");

  pragma "no doc"
  /* See :ref:`readme-chplenv.CHPL_JEMALLOC` for more information. */
  param CHPL_JEMALLOC:string           = __primitive("get compiler variable", "CHPL_JEMALLOC");

  /* See :ref:`readme-chplenv.CHPL_REGEXP` for more information. */
  param CHPL_REGEXP:string          = __primitive("get compiler variable", "CHPL_REGEXP");

  /* See :ref:`readme-chplenv.CHPL_LLVM` for more information. */
  param CHPL_LLVM:string            = __primitive("get compiler variable", "CHPL_LLVM");
}
