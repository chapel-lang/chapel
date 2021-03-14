/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

  public use LocaleModelHelpNUMA;
  public use LocaleModelHelpMem;

  use IO;

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

  //
  // The NUMA sublocale model
  //
  class NumaDomain : AbstractLocaleModel {
    const sid: chpl_sublocID_t;
    const ndName: string; // note: locale provides `proc name`

    // top-level node id
    override proc chpl_id(){
      return (parent._instance: borrowed LocaleModel?)!._node_id;
    }
    override proc chpl_localeid() {
      return chpl_buildLocaleID(
          (parent._instance: borrowed LocaleModel?)!._node_id:chpl_nodeID_t,
          sid);
    }
    override proc chpl_name() return ndName;

    proc init() {
    }

    proc init(_sid, _parent) {
      super.init(_parent);
      sid = _sid;
      ndName = "ND"+sid:string;
    }

    override proc writeThis(f) throws {
      if parent._instance then
        parent.writeThis(f);
      f <~> '.'+ndName;
    }

    override proc getChildCount(): int { return 0; }
    iter getChildIndices() : int {
      halt("No children to iterate over.");
      yield -1;
    }
    proc addChild(loc:locale) {
      halt("Cannot add children to this locale type.");
    }
    pragma "unsafe"
    override proc getChild(idx:int) : locale {
      halt("Cannot getChild with this locale type");
      var ret: locale; // default-initialize
      return ret;
    }

    iter getChildren() : locale {
      halt("No children to iterate over.");
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

    const numSublocales: int;
    const childSpace: domain(1);
    // Todo: avoid the pragma by having helpSetupLocaleNUMA return this array
    // and initialize the field from it. Need childSpace to be const?
    pragma "unsafe"
    var childLocales: [childSpace] unmanaged NumaDomain;

    // This constructor must be invoked "on" the node
    // that it is intended to represent.  This trick is used
    // to establish the equivalence the "locale" field of the locale object
    // and the node ID portion of any wide pointer referring to it.
    proc init() {
      use SysCTypes;
      _node_id = chpl_nodeID: int;
      extern proc chpl_topo_getNumNumaDomains(): c_int;
      numSublocales = chpl_topo_getNumNumaDomains();
      childSpace = {0..#numSublocales};

      if rootLocaleInitialized {
        halt("Cannot create additional LocaleModel instances");
      }

      this.complete();

      setup();
    }

    proc init(parent_loc : locale) {
      if rootLocaleInitialized {
        halt("Cannot create additional LocaleModel instances");
      }
      super.init(parent_loc);

      // Why doesn't this work (generates an internal error) to avoid
      // the code duplication below?  Or if it's not supposed to work,
      // how could we make it a user error?
      //
      //      this.init();

      use SysCTypes;
      _node_id = chpl_nodeID: int;
      extern proc chpl_topo_getNumNumaDomains(): c_int;
      numSublocales = chpl_topo_getNumNumaDomains();
      childSpace = {0..#numSublocales};

      this.complete();

      setup();
    }

    override proc chpl_id() return _node_id;     // top-level locale (node) number
    override proc chpl_localeid() {
      return chpl_buildLocaleID(_node_id:chpl_nodeID_t, c_sublocid_any);
    }
    override proc chpl_name() return local_name;

    //
    // Support for different types of memory:
    // large, low latency, and high bandwidth
    //
    // The numa memory model currently assumes only one memory.
    //
    // ENGIN: Are these ever used?
    override proc defaultMemory() : locale {
      return new locale(this);
    }

    override proc largeMemory() : locale {
      return new locale(this);
    }

    override proc lowLatencyMemory() : locale {
      return new locale(this);
    }

    override proc highBandwidthMemory() : locale {
      return new locale(this);
    }

    proc getChildSpace() return childSpace;

    override proc getChildCount() return numSublocales;

    iter getChildIndices() : int {
      for idx in childSpace do
        yield idx;
    }

    override proc getChild(idx:int) : locale {
      if boundsChecking then
        if (idx < 0) || (idx >= numSublocales) then
          halt("sublocale child index out of bounds (",idx,")");
      return new locale(childLocales[idx]);
    }

    iter getChildren() : locale  {
      for loc in childLocales do
        yield new locale(loc);
    }

    proc getChildArray() {
      return [loc in childLocales] loc;
    }

    //------------------------------------------------------------------------{
    //- Implementation (private)
    //-
    proc setup() {
      helpSetupLocaleNUMA(this, local_name, numSublocales, NumaDomain);
    }
    //------------------------------------------------------------------------}

    // ENGIN: We store all LocaleModel instances in the Locales array which is
    // marked "locale private" locale private variables are autoDestroy'd by the
    // compiler. So, nothing to deinit here.
    proc deinit() { }
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
      super.init(nilLocale);
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
      helpSetupRootLocaleNUMA(this);
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

    override proc getChildCount() return this.myLocaleSpace.size;

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
      if chpl_isActualSublocID(subloc) then
        return (myLocales[node:int].getChild(subloc:int)):locale;
      else {
        const n = node:int;
        const l = myLocales[n];
        return l:locale;
      }
    }

    proc deinit() {
      for loc in myLocales {
        on loc {
          rootLocaleInitialized = false;
        }
      }
    }
  }
}
