/*
 * Copyright 2017 Advanced Micro Devices, Inc.
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
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

// LocaleModel.chpl
//
// This provides a flat locale model architectural description.  The
// locales contain memory and a multi-core processor with homogeneous
// cores, and we ignore any affinity (NUMA effects) between the
// processor cores and the memory.  This architectural description is
// backward compatible with the architecture implicitly provided by
// releases 1.6 and preceding.
//
module LocaleModel {

  use LocaleModelHelpAPU;
  use LocaleModelHelpMem;

  private use IO;

  //
  // The task layer calls these to convert between full sublocales and
  // execution sublocales.  Full sublocales may contain more information
  // in some locale models, but not in this one.
  //
  export
  proc chpl_localeModel_sublocToExecutionSubloc(full_subloc:chpl_sublocID_t)
  {
    return full_subloc;  // execution sublocale is same as full sublocale
  }

  export
  proc chpl_localeModel_sublocMerge(full_subloc:chpl_sublocID_t,
                                    execution_subloc:chpl_sublocID_t)
  {
    return execution_subloc;  // no info needed from full sublocale
  }

  class CPULocale : AbstractLocaleModel {
    const sid: chpl_sublocID_t;
    const name: string;

    override proc chpl_id() return (parent:LocaleModel)._node_id; // top-level node id
    override proc chpl_localeid() {
      return chpl_buildLocaleID((parent:LocaleModel)._node_id:chpl_nodeID_t,
                                sid);
    }
    override proc chpl_name() return name;

    proc init() {
    }

    proc init(_sid, _parent) {
      super.init(_parent);
      sid = _sid;
      name = _parent.chpl_name() + "-CPU" + sid:string;
    }

    override proc writeThis(f) throws {
      parent.writeThis(f);
      f <~> '.'+name;
    }

    override proc getChildCount(): int { return 0; }
    iter getChildIndices() : int {
      halt("No children to iterate over.");
      yield -1;
    }
    proc addChild(loc:locale) {
      halt("Cannot add children to this locale type.");
    }
    override proc getChild(idx:int) : locale {
      halt("requesting a child from a CPULocale locale");
      return this;
    }
  }

  class GPULocale : AbstractLocaleModel {
    const sid: chpl_sublocID_t;
    const name: string;

    override proc chpl_id() return (parent:LocaleModel)._node_id; // top-level node id
    override proc chpl_localeid() {
      return chpl_buildLocaleID((parent:LocaleModel)._node_id:chpl_nodeID_t,
                                sid);
    }
    override proc chpl_name() return name;

    proc init() {
    }
    proc deinit() {
     extern proc hsa_shutdown(): void ;

     // Comment out this until runtime support for HSA is added

     //     hsa_shutdown();
    }


    proc init(_sid, _parent) {
      super.init(_parent);
      sid = _sid;
      name = _parent.chpl_name() + "-GPU" + sid:string;
    }

    override proc writeThis(f) throws {
      parent.writeThis(f);
      f <~> '.'+name;
    }

    override proc getChildCount(): int { return 0; }
    iter getChildIndices() : int {
      halt("No children to iterate over.");
      yield -1;
    }
    proc addChild(loc:locale) {
      halt("Cannot add children to this locale type.");
    }
    override proc getChild(idx:int) : locale {
      halt("requesting a child from a GPULocale locale");
      return this;
    }
  }

  const chpl_emptyLocaleSpace: domain(1) = {1..0};
  pragma "unsafe"
  const chpl_emptyLocales: [chpl_emptyLocaleSpace] locale;

  //
  // The node model
  //
  class LocaleModel : AbstractLocaleModel {
    const _node_id : int;
    var local_name : string; // should never be modified after first assignment

    var numSublocales: int; // should never be modified after first assignment
    var GPU: GPULocale;
    var CPU: CPULocale;

    // This constructor must be invoked "on" the node
    // that it is intended to represent.  This trick is used
    // to establish the equivalence the "locale" field of the locale object
    // and the node ID portion of any wide pointer referring to it.
    proc init() {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      _node_id = chpl_nodeID: int;

      this.complete();

      setup();
    }

    proc init(parent_loc : locale) {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      super.init(parent_loc);

      _node_id = chpl_nodeID: int;

      this.complete();

      setup();
    }

    override proc chpl_id() return _node_id;     // top-level locale (node) number
    override proc chpl_localeid() {
      return chpl_buildLocaleID(_node_id:chpl_nodeID_t, c_sublocid_any);
    }
    override proc chpl_name() return local_name;

    proc getChildSpace() {
      halt("error!");
      return {0..#numSublocales};
    }

    override proc getChildCount() return 0;

    iter getChildIndices() : int {
      for idx in {0..#numSublocales} do // chpl_emptyLocaleSpace do
        yield idx;
    }

    override proc getChild(idx:int) : locale {
      if idx == 1
        then return GPU;
      else
        return CPU;
    }

    iter getChildren() : locale  {
      halt("in here");
      for idx in {0..#numSublocales} {
        if idx == 1
          then yield GPU;
        else
          yield CPU;
      }
    }

    proc getChildArray() {
      halt ("in get child Array");
      return chpl_emptyLocales;
    }


    //------------------------------------------------------------------------{
    //- Implementation (private)
    //-
    proc setup() {
      helpSetupLocaleAPU(this, local_name, numSublocales, CPULocale, GPULocale);
    }
    //------------------------------------------------------------------------}

    proc deinit() {
      if (debugAPULocale) {
        chpl_debug_writeln("** Destructing CPU/GPU locales and shutting down HSA.");
      }
      delete _to_unmanaged(CPU);
      delete _to_unmanaged(GPU);
    }
 }

  //
  // An instance of this class is the default contents 'rootLocale'.
  //
  // In the current implementation a platform-specific architectural description
  // may overwrite this instance or any of its children to establish a more customized
  // representation of the system resources.
  //
  class RootLocale : AbstractRootLocale {

    const myLocaleSpace: domain(1) = {0..numLocales-1};
    pragma "unsafe"
    var myLocales: [myLocaleSpace] locale;

    proc init() {
      super.init(nil);
      nPUsPhysAcc = 0;
      nPUsPhysAll = 0;
      nPUsLogAcc = 0;
      nPUsLogAll = 0;
      maxTaskPar = 0;
    }

    // The setup() function must use chpl_initOnLocales() to iterate (in
    // parallel) over the locales to set up the LocaleModel object.
    // In addition, the initial 'here' must be set.
    proc setup() {
      helpSetupRootLocaleAPU(this);
    }

    // Has to be globally unique and not equal to a node ID.
    // We return numLocales for now, since we expect nodes to be
    // numbered less than this.
    // -1 is used in the abstract locale class to specify an invalid node ID.
    override proc chpl_id() return numLocales;
    override proc chpl_localeid() {
      return chpl_buildLocaleID(numLocales:chpl_nodeID_t, c_sublocid_none);
    }
    override proc chpl_name() return local_name();
    proc local_name() return "rootLocale";

    override proc writeThis(f) throws {
      f <~> name;
    }

    override proc getChildCount() return this.myLocaleSpace.numIndices;

    proc getChildSpace() return this.myLocaleSpace;

    iter getChildIndices() : int {
      for idx in this.myLocaleSpace do
        yield idx;
    }

    override proc getChild(idx:int) return this.myLocales[idx];

    iter getChildren() : locale  {
      for loc in this.myLocales do
        yield loc;
    }

    override proc getDefaultLocaleSpace() const ref return this.myLocaleSpace;
    override proc getDefaultLocaleArray() const ref return myLocales;

    override proc localeIDtoLocale(id : chpl_localeID_t) {
      const node = chpl_nodeFromLocaleID(id);
      const subloc = chpl_sublocFromLocaleID(id);
      if (subloc == c_sublocid_none) || (subloc == c_sublocid_any) then
        return (myLocales[node:int]):locale;
      else
        return (myLocales[node:int].getChild(subloc:int)):locale;
    }

    proc deinit() {
      for loc in myLocales {
        on loc {
          rootLocaleInitialized = false;
          delete _to_unmanaged(loc);
        }
      }
    }
  }
}
