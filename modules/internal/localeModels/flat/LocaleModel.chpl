/*
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

  use LocaleModelHelpFlat;
  use LocaleModelHelpMem;

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

  const chpl_emptyLocaleSpace: domain(1) = {1..0};
  const chpl_emptyLocales: [chpl_emptyLocaleSpace] locale;

  //
  // A concrete class representing the nodes in this architecture.
  //
  class LocaleModel : AbstractLocaleModel {
    const _node_id : int;
    const local_name : string;

    // This constructor must be invoked "on" the node
    // that it is intended to represent.  This trick is used
    // to establish the equivalence the "locale" field of the locale object
    // and the node ID portion of any wide pointer referring to it.
    proc init() {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      setup();
    }

    proc init(parent_loc : locale) {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      super.init(parent_loc);
      setup();
    }

    proc chpl_id() return _node_id;     // top-level locale (node) number
    proc chpl_localeid() {
      return chpl_buildLocaleID(_node_id:chpl_nodeID_t, c_sublocid_any);
    }
    proc chpl_name() return local_name;

    //
    // Support for different types of memory:
    // large, low latency, and high bandwidth
    //
    // The flat memory model assumes only one memory.
    //
    proc defaultMemory() : locale {
      return this;
    }

    proc largeMemory() : locale {
      return this;
    }

    proc lowLatencyMemory() : locale {
      return this;
    }

    proc highBandwidthMemory() : locale {
      return this;
    }


    proc writeThis(f) {
      // Most classes will define it like this:
      //      f <~> name;
      // but here it is defined thus for backward compatibility.
      f <~> new ioLiteral("LOCALE") <~> _node_id;
    }

    proc getChildSpace() return chpl_emptyLocaleSpace;

    proc getChildCount() return 0;

    iter getChildIndices() : int {
      for idx in chpl_emptyLocaleSpace do
        yield idx;
    }

    proc getChild(idx:int) : locale {
      if boundsChecking then
        halt("requesting a child from a LocaleModel locale");
      return nil;
    }

    iter getChildren() : locale  {
      for loc in chpl_emptyLocales do
        yield loc;
    }

    proc getChildArray() {
      return chpl_emptyLocales;
    }

    //------------------------------------------------------------------------{
    //- Implementation (private)
    //-
    proc setup() {
      _node_id = chpl_nodeID: int;

      helpSetupLocaleFlat(this, local_name);
    }
    //------------------------------------------------------------------------}
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
      helpSetupRootLocaleFlat(this);
    }

    // Has to be globally unique and not equal to a node ID.
    // We return numLocales for now, since we expect nodes to be
    // numbered less than this.
    // -1 is used in the abstract locale class to specify an invalid node ID.
    proc chpl_id() return numLocales;
    proc chpl_localeid() {
      return chpl_buildLocaleID(numLocales:chpl_nodeID_t, c_sublocid_none);
    }
    proc chpl_name() return local_name();
    proc local_name() return "rootLocale";

    proc writeThis(f) {
      f <~> name;
    }

    proc getChildCount() return this.myLocaleSpace.numIndices;

    proc getChildSpace() return this.myLocaleSpace;

    iter getChildIndices() : int {
      for idx in this.myLocaleSpace do
        yield idx;
    }

    proc getChild(idx:int) return this.myLocales[idx];

    iter getChildren() : locale  {
      for loc in this.myLocales do
        yield loc;
    }

    proc getDefaultLocaleSpace() const ref return this.myLocaleSpace;
    proc getDefaultLocaleArray() const ref return myLocales;

    proc localeIDtoLocale(id : chpl_localeID_t) {
      // In the default architecture, there are only nodes and no sublocales.
      // What is more, the nodeID portion of a wide pointer is the same as
      // the index into myLocales that yields the locale representing that
      // node.
      return myLocales[chpl_rt_nodeFromLocaleID(id)];
    }

    proc deinit() {
      for loc in myLocales do
        delete loc;
    }
  }

  //////////////////////////////////////////
  //
  // utilities
  //
  inline
  proc chpl_getSubloc() {
    halt("called chpl_getSubloc() in a locale model that lacks sublocales");
    return c_sublocid_none;
  }
}
