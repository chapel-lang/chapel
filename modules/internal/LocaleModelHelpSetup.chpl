/*
 * Copyright 2017 Advanced Micro Devices, Inc.
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
  use ChapelEnv;
  use Sys;
  private use SysCTypes;

  config param debugLocaleModel = false;

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
    proc setRootLocaleValues(dst:borrowed RootLocale) {
      dst.nPUsPhysAcc = nPUsPhysAcc.read();
      dst.nPUsPhysAll = nPUsPhysAll.read();
      dst.nPUsLogAcc = nPUsLogAcc.read();
      dst.nPUsLogAll = nPUsLogAll.read();
      dst.maxTaskPar = maxTaskPar.read();
    }
  }

  proc helpSetupRootLocaleFlat(dst:borrowed RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() with (ref root_accum) {
      const node = new locale(new unmanaged LocaleModel(new locale(dst)));
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
  }

  proc helpSetupRootLocaleNUMA(dst:borrowed RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() with (ref root_accum) {
      chpl_task_setSubloc(c_sublocid_any);
      const node = new locale(new unmanaged LocaleModel(new locale (dst)));
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
  }

  proc helpSetupRootLocaleAPU(dst:borrowed RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() with (ref root_accum) {
      chpl_task_setSubloc(c_sublocid_any);
      const node = new locale(new unmanaged LocaleModel(new locale(dst)));
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
    here.runningTaskCntSet(0);  // locale init parallelism mis-sets this
  }

  // gasnet-smp and gasnet-udp w/ GASNET_SPAWNFN=L are local spawns
  private inline proc localSpawn() {
    if CHPL_COMM == "gasnet" {
      var spawnfn: c_string;
      if (CHPL_COMM_SUBSTRATE == "udp" &&
         sys_getenv(c"GASNET_SPAWNFN", spawnfn) == 1 && spawnfn == c"L") {
        return true;
      } else if (CHPL_COMM_SUBSTRATE == "smp") {
        return true;
      }
    }
    return false;
  }

  private inline proc getNodeName() {
    // chpl_nodeName is defined in chplsys.c.
    // It supplies a node name obtained by running uname(3) on the
    // current node.  For this reason (as well), the constructor (or
    // at least this setup method) must be run on the node it is
    // intended to describe.
    extern proc chpl_nodeName(): c_string;
    var _node_name: string;
    try! {
      _node_name = createStringWithNewBuffer(chpl_nodeName());
    }
    const _node_id = (chpl_nodeID: int): string;

    return if localSpawn() then _node_name + "-" + _node_id else _node_name;
  }

  proc helpSetupLocaleFlat(dst:borrowed LocaleModel, out local_name:string) {
    local_name = getNodeName();

    extern proc chpl_task_getCallStackSize(): size_t;
    dst.callStackSize = chpl_task_getCallStackSize();

    extern proc chpl_topo_getNumCPUsPhysical(accessible_only: bool): c_int;
    dst.nPUsPhysAcc = chpl_topo_getNumCPUsPhysical(true);
    dst.nPUsPhysAll = chpl_topo_getNumCPUsPhysical(false);

    extern proc chpl_topo_getNumCPUsLogical(accessible_only: bool): c_int;
    dst.nPUsLogAcc = chpl_topo_getNumCPUsLogical(true);
    dst.nPUsLogAll = chpl_topo_getNumCPUsLogical(false);

    extern proc chpl_task_getMaxPar(): uint(32);
    dst.maxTaskPar = chpl_task_getMaxPar();
  }

  proc helpSetupLocaleNUMA(dst:borrowed LocaleModel, out local_name:string, numSublocales, type NumaDomain) {
    helpSetupLocaleFlat(dst, local_name);

    extern proc chpl_task_getMaxPar(): uint(32);

    if numSublocales >= 1 {
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
        dst.childLocales[i] = new unmanaged NumaDomain(i:chpl_sublocID_t,
                                                       new locale(dst));
        dst.childLocales[i].nPUsPhysAcc = nPUsPhysAccPerSubloc;
        dst.childLocales[i].nPUsPhysAll = nPUsPhysAllPerSubloc;
        dst.childLocales[i].nPUsLogAcc = nPUsLogAccPerSubloc;
        dst.childLocales[i].nPUsLogAll = nPUsLogAllPerSubloc;
        dst.childLocales[i].maxTaskPar = maxTaskParPerSubloc;
      }
      chpl_task_setSubloc(origSubloc);
    }
  }

  proc helpSetupLocaleAPU(dst:borrowed LocaleModel, out local_name:string, out
      numSublocales, type CPULocale, type GPULocale) {
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
    dst.CPU = new unmanaged CPULocale(0:chpl_sublocID_t, dst);

    chpl_task_setSubloc(1:chpl_sublocID_t);

    dst.GPU = new unmanaged GPULocale(1:chpl_sublocID_t, dst);
    chpl_task_setSubloc(origSubloc);
  }
}
