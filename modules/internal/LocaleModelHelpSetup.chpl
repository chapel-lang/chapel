/*
 * Copyright 2004-2016 Cray Inc.
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
// Provides for declarations common to locale models
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

    forall locIdx in dst.chpl_initOnLocales() {
      const node = new LocaleModel(dst);
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
    here.runningTaskCntSet(0);  // locale init parallelism mis-sets this
  }

  proc helpSetupRootLocaleNUMA(dst:RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() {
      chpl_task_setSubloc(c_sublocid_any);
      const node = new LocaleModel(dst);
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
    here.runningTaskCntSet(0);  // locale init parallelism mis-sets this
  }

}
