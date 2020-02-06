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

// ChapelThreads.chpl
//
// numThreadsPerLocale is used to tell the task layer how many threads
// to use to execute the user's tasks.  The interpretation of the
// value varies depending on task layer and is documented in tasks.rst
// in the release documentation directory.  The sentinel value of 0
// indicates that the tasking layer can determine the number of
// threads to use.
//
module ChapelThreads
{
  extern proc chpl_task_getenvNumThreadsPerLocale(): int(32);

  const numThreadsPerLocale: int = chpl_task_getenvNumThreadsPerLocale();
}
