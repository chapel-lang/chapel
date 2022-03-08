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

  // Internal modules.
  public use CString as CString;
  public use Bytes as Bytes;
  public use String as String;
  public use OwnedObject as OwnedObject;
  public use SharedObject as SharedObject;
  public use ChapelEnv as ChapelEnv;
  public use ChapelBase as ChapelBase;
  public use Atomics as Atomics;
  public use NetworkAtomics as NetworkAtomics;
  public use NetworkAtomicTypes as NetworkAtomicTypes;
  public use AtomicsCommon as AtomicsCommon;
  public use ChapelIteratorSupport as ChapelIteratorSupport;
  public use ChapelThreads as ChapelThreads;
  public use ChapelTuple as ChapelTuple;
  public use ChapelRange as ChapelRange;
  public use ChapelReduce as ChapelReduce;
  public use ChapelSyncvar as ChapelSyncvar;
  public use ChapelTaskDataHelp as ChapelTaskDataHelp;
  public use LocaleModel as _; // let LocaleModel refer to the class
  public use ChapelLocale as ChapelLocale;
  public use ChapelPrivatization as ChapelPrivatization;
  public use DefaultRectangular as DefaultRectangular; // This might be able to go just after Atomics
  public use LocalesArray as LocalesArray;
  public use ChapelArray as ChapelArray;
  public use ChapelDistribution as ChapelDistribution;
  public use ChapelAutoLocalAccess as ChapelAutoLocalAccess;
  public use ChapelIO as ChapelIO;
  public use LocaleTree as LocaleTree;
  public use ChapelHashing as ChapelHashing;
  public use DefaultAssociative as DefaultAssociative;
  public use DefaultSparse as DefaultSparse;
  public use ChapelTaskID as ChapelTaskID;
  public use ChapelTaskTable as ChapelTaskTable;
  public use MemTracking as MemTracking;
  public use ChapelUtil as ChapelUtil;
  public use Errors as Errors;
  public use ChapelTaskData as ChapelTaskData;
  public use ChapelSerializedBroadcast as ChapelSerializedBroadcast;
  public use ExportWrappers as ExportWrappers;
  public use ChapelAutoAggregation as ChapelAutoAggregation;

  // Standard modules.
  public use Types as Types;
  public use Math as Math;
  public use VectorizingIterator as VectorizingIterator;

  use stopInitCommDiags;  // Internal, but uses standard/CommDiagnostics
}
