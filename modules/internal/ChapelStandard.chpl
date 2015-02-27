/*
 * Copyright 2004-2015 Cray Inc.
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

// ChapelStandard.chpl
//
pragma "export init"
module ChapelStandard {
  use startInitCommDiags;

  // Internal modules.
  use ChapelBase;
  use CString;
  use String;
  use MemConsistency;
  use Atomics;
  use NetworkAtomics;
  use NetworkAtomicTypes;
  use AtomicsCommon;
  use ChapelIteratorSupport;
  use ChapelThreads;
  use ChapelThreadsInternal;
  use ChapelTasksInternal;
  use ChapelTuple;
  use ChapelRange;
  use ChapelReduce;
  use ChapelSyncvar;
  use LocaleModel;
  use ChapelLocale;
  use DefaultRectangular; // This might be able to go just after Atomics
  use LocalesArray;
  use ChapelArray;
  use ChapelDistribution;
  use ChapelIO;
  use LocaleTree;
  use DefaultAssociative;
  use DefaultSparse;
  use DefaultOpaque;
  use ChapelTaskID;
  use ChapelTaskTable;
  use MemTracking;
  use ChapelUtil;
  use ChapelDynDispHack;

  // Standard modules.
  use Assert;
  use Types;
  use Math;

  use stopInitCommDiags;
}
