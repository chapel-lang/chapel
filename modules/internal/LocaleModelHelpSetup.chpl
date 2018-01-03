/*
 * Copyright 2017 Advanced Micro Devices, Inc.
 * Copyright 2004-2018 Cray Inc.
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

// LocaleModelHelpSetup.chpl
//
// Provides for declarations common to locale model setup
// but that do not have to be the same in order to meet the
// interface.

// They are in this file as a practical matter to avoid code
// duplication. If necessary, a locale model using this file
// should feel free to reimplement them in some other way.
module LocaleModelHelpSetup {

  use ChapelLocale;
  use DefaultRectangular;
  use ChapelNumLocales;
  use Sys;

  config param debugLocaleModel = false;

  var doneCreatingLocales: bool = false;

  extern var chpl_nodeID: chpl_nodeID_t;

  record chpl_root_locale_accum {
    var nPUsPhysAcc: atomic int;
    var nPUsPhysAll: atomic int;
    var nPUsLogAcc: atomic int;
    var nPUsLogAll: atomic int;
    var maxTaskPar: atomic int;

    proc accum(loc:locale) {
      nPUsPhysAcc.add(loc.nPUsPhysAcc);
      nPUsPhysAll.add(loc.nPUsPhysAll);
      nPUsLogAcc.add(loc.nPUsLogAcc);
      nPUsLogAll.add(loc.nPUsLogAll);
      maxTaskPar.add(loc.maxTaskPar);
    }
    proc setRootLocaleValues(dst:RootLocale) {
      dst.nPUsPhysAcc = nPUsPhysAcc.read();
      dst.nPUsPhysAll = nPUsPhysAll.read();
      dst.nPUsLogAcc = nPUsLogAcc.read();
      dst.nPUsLogAll = nPUsLogAll.read();
      dst.maxTaskPar = maxTaskPar.read();
    }
  }

  proc helpSetupRootLocaleFlat(dst:RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() with (ref root_accum) {
      const node = new LocaleModel(dst);
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
  }

  proc helpSetupRootLocaleNUMA(dst:RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() with (ref root_accum) {
      chpl_task_setSubloc(c_sublocid_any);
      const node = new LocaleModel(dst);
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
  }

  proc helpSetupRootLocaleAPU(dst:RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() with (ref root_accum) {
      chpl_task_setSubloc(c_sublocid_any);
      const node = new LocaleModel(dst);
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
    here.runningTaskCntSet(0);  // locale init parallelism mis-sets this
  }

  proc helpSetupLocaleFlat(dst:LocaleModel, out local_name:string) {
    const _node_id = chpl_nodeID: int;

    // chpl_nodeName is defined in chplsys.c.
    // It supplies a node name obtained by running uname(3) on the
    // current node.  For this reason (as well), the constructor (or
    // at least this setup method) must be run on the node it is
    // intended to describe.
    var comm, spawnfn : c_string;
    extern proc chpl_nodeName() : c_string;
    // sys_getenv returns zero on success.
    if sys_getenv(c"CHPL_COMM", comm) == 0 && comm == c"gasnet" &&
      sys_getenv(c"GASNET_SPAWNFN", spawnfn) == 0 && spawnfn == c"L"
    then local_name = chpl_nodeName():string + "-" + _node_id:string;
    else local_name = chpl_nodeName():string;

    extern proc chpl_task_getCallStackSize(): size_t;
    dst.callStackSize = chpl_task_getCallStackSize();

    extern proc chpl_getNumPhysicalCpus(accessible_only: bool): c_int;
    dst.nPUsPhysAcc = chpl_getNumPhysicalCpus(true);
    dst.nPUsPhysAll = chpl_getNumPhysicalCpus(false);

    extern proc chpl_getNumLogicalCpus(accessible_only: bool): c_int;
    dst.nPUsLogAcc = chpl_getNumLogicalCpus(true);
    dst.nPUsLogAll = chpl_getNumLogicalCpus(false);

    extern proc chpl_task_getMaxPar(): uint(32);
    dst.maxTaskPar = chpl_task_getMaxPar();
  }

  proc helpSetupLocaleNUMA(dst:LocaleModel, out local_name:string, out numSublocales) {
    helpSetupLocaleFlat(dst, local_name);

    extern proc chpl_task_getNumSublocales(): int(32);
    numSublocales = chpl_task_getNumSublocales();

    extern proc chpl_task_getMaxPar(): uint(32);

    if numSublocales >= 1 {
      dst.childSpace = {0..#numSublocales};
      // These nPUs* values are estimates only; better values await
      // full hwloc support. In particular it assumes a homogeneous node
      const nPUsPhysAccPerSubloc = dst.nPUsPhysAcc/numSublocales;
      const nPUsPhysAllPerSubloc = dst.nPUsPhysAll/numSublocales;
      const nPUsLogAccPerSubloc = dst.nPUsLogAcc/numSublocales;
      const nPUsLogAllPerSubloc = dst.nPUsLogAll/numSublocales;
      const maxTaskParPerSubloc = chpl_task_getMaxPar()/numSublocales;
      const origSubloc = chpl_task_getRequestedSubloc(); // this should be any
      for i in dst.childSpace {
        // allocate the structure on the proper sublocale
        chpl_task_setSubloc(i:chpl_sublocID_t);
        dst.childLocales[i] = new NumaDomain(i:chpl_sublocID_t, dst);
        dst.childLocales[i].nPUsPhysAcc = nPUsPhysAccPerSubloc;
        dst.childLocales[i].nPUsPhysAll = nPUsPhysAllPerSubloc;
        dst.childLocales[i].nPUsLogAcc = nPUsLogAccPerSubloc;
        dst.childLocales[i].nPUsLogAll = nPUsLogAllPerSubloc;
        dst.childLocales[i].maxTaskPar = maxTaskParPerSubloc;
      }
      chpl_task_setSubloc(origSubloc);
    }
  }

  proc helpSetupLocaleAPU(dst:LocaleModel, out local_name:string, out numSublocales) {
    helpSetupLocaleFlat(dst, local_name);

    extern proc chpl_task_getMaxPar(): uint(32);

    // Comment out HSA initialization until runtime HSA support is checked in

    //    extern proc chpl_hsa_initialize(): c_int;
    //    var initHsa =  chpl_hsa_initialize();
    //    if (initHsa == 1) {
    //      halt("Could not initialize HSA");
    //    }

    // Hardcode two sublocales, 1 CPU and 1 GPU
    numSublocales = 2;

    const origSubloc = chpl_task_getRequestedSubloc();

    chpl_task_setSubloc(0:chpl_sublocID_t);
    dst.CPU = new CPULocale(0:chpl_sublocID_t, dst);

    chpl_task_setSubloc(1:chpl_sublocID_t);

    dst.GPU = new GPULocale(1:chpl_sublocID_t, dst);
    chpl_task_setSubloc(origSubloc);
  }
}
