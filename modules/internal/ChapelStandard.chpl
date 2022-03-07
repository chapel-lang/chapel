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
  public import CString;
  public use CString;
  public import Bytes;
  public use Bytes;
  public import String;
  public use String;
  public import OwnedObject;
  public use OwnedObject;
  public import SharedObject;
  public use SharedObject;
  public import ChapelEnv;
  public use ChapelEnv;
  public import ChapelBase;
  public use ChapelBase;
  public import Atomics;
  public use Atomics;
  public import NetworkAtomics;
  public use NetworkAtomics;
  public import NetworkAtomicTypes;
  public use NetworkAtomicTypes;
  public import AtomicsCommon;
  public use AtomicsCommon;
  public import ChapelIteratorSupport;
  public use ChapelIteratorSupport;
  public import ChapelThreads;
  public use ChapelThreads;
  public import ChapelTuple;
  public use ChapelTuple;
  public import ChapelRange;
  public use ChapelRange;
  public import ChapelReduce;
  public use ChapelReduce;
  public import ChapelSyncvar;
  public use ChapelSyncvar;
  public import ChapelTaskDataHelp;
  public use ChapelTaskDataHelp;
  public use LocaleModel as _; // let LocaleModel refer to the class
  public import ChapelLocale;
  public use ChapelLocale;
  public import ChapelPrivatization;
  public use ChapelPrivatization;
  public import DefaultRectangular; // This might be able to go just after Atomics
  public use DefaultRectangular; // This might be able to go just after Atomics
  public import LocalesArray;
  public use LocalesArray;
  public import ChapelArray;
  public use ChapelArray;
  public import ChapelDistribution;
  public use ChapelDistribution;
  public import ChapelAutoLocalAccess;
  public use ChapelAutoLocalAccess;
  public import ChapelIO;
  public use ChapelIO;
  public import LocaleTree;
  public use LocaleTree;
  public import ChapelHashing;
  public use ChapelHashing;
  public import DefaultAssociative;
  public use DefaultAssociative;
  public import DefaultSparse;
  public use DefaultSparse;
  public import ChapelTaskID;
  public use ChapelTaskID;
  public import ChapelTaskTable;
  public use ChapelTaskTable;
  public import MemTracking;
  public use MemTracking;
  public import ChapelUtil;
  public use ChapelUtil;
  public import Errors;
  public use Errors;
  public import ChapelTaskData;
  public use ChapelTaskData;
  public import ChapelSerializedBroadcast;
  public use ChapelSerializedBroadcast;
  public import ExportWrappers;
  public use ExportWrappers;
  public import ChapelAutoAggregation;
  public use ChapelAutoAggregation;

  // Standard modules.
  public import Types;
  public use Types;
  public import Math;
  public use Math;
  public import VectorizingIterator;
  public use VectorizingIterator;

  use stopInitCommDiags;  // Internal, but uses standard/CommDiagnostics
}
