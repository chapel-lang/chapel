/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

  // The following list 'use's the internal modules whose contents
  // should be available to every program (as well as some automatic
  // standard modules that have moved around over time).  Note that
  // these 'public use's make the contents of these modules available
  // to 'use's of this module, but hide the name of the module itself
  // (unless 'as xyz;' is also added).  Issue #19793 suggests taking
  // this a step further and not permitting the user to refer to the
  // names of internal modules at all.
  public use CString;
  public use Bytes;
  public use String;
  public use OwnedObject;
  public use SharedObject;
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
  public use ChapelPrivatization;
  public use DefaultRectangular;
  public use LocalesArray;
  public use ChapelArray;
  public use ChapelDistribution;
  public use ChapelAutoLocalAccess;
  public use ChapelIO as ChapelIO;
  public use LocaleTree;
  public use ChapelHashing;
  public use DefaultAssociative;
  public use DefaultSparse;
  public use ChapelTaskID;
  public use ChapelTaskTable;
  public use MemTracking;
  public use ChapelUtil;
  public use Errors as Errors;
  public use ChapelTaskData;
  public use ChapelSerializedBroadcast;
  public use ExportWrappers;
  public use ChapelAutoAggregation;

  // Standard modules.
  public use Types as Types;
  public use AutoMath as AutoMath;

  use stopInitCommDiags;  // Internal, but uses standard/CommDiagnostics
}
