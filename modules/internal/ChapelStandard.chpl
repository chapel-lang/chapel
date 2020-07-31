/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

// ChapelStandard.chpl
//
pragma "export init"
module ChapelStandard {
  use startInitCommDiags;
  // Internal, but uses standard/CommDiagnostics

  // Internal modules.
  public use CString;
  public use Bytes;
  public use String;
  public use OwnedObject;
  public use SharedObject;
  public use ChapelEnv;
  public use ChapelBase;
  public use Atomics;
  public use NetworkAtomics;
  public use NetworkAtomicTypes;
  public use AtomicsCommon;
  public use ChapelIteratorSupport;
  public use ChapelThreads;
  public use ChapelTuple;
  public use ChapelRange;
  public use ChapelReduce;
  public use ChapelSyncvar;
  public use ChapelTaskDataHelp;
  public use LocaleModel;
  public use ChapelLocale;
  public use DefaultRectangular; // This might be able to go just after Atomics
  public use LocalesArray;
  public use ChapelArray;
  public use ChapelDistribution;
  public use ChapelAutoLocalAccess;
  public use ChapelIO;
  public use LocaleTree;
  public use ChapelHashing;
  public use DefaultAssociative;
  public use DefaultSparse;
  public use ChapelTaskID;
  public use ChapelTaskTable;
  public use MemTracking;
  public use ChapelUtil;
  public use ChapelError;
  public use ChapelTaskData;
  public use ChapelSerializedBroadcast;
  public use ExportWrappers;

  // Standard modules.
  public use Builtins;
  public use Types;
  public use Math;

  use stopInitCommDiags;  // Internal, but uses standard/CommDiagnostics
}
