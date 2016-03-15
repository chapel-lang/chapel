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

  // localeModelHasSublocales
  // moved to LocaleModelHelpFlat.chpl

  use LocaleModelHelpFlat;

  // debugLocaleModel
  // doneCreatingLocales
  // chpl_localeID_t
  // chpl__initCopy for chpl_localeID_t
  // chpl_nodeID
  // chpl_rt_buildLocaleID
  // chpl_rt_nodeFromLocaleID
  // chpl_rt_sublocFromLocaleID
  // chpl_buildLocaleID
  // chpl_nodeFromLocaleID
  // chpl_sublocFromLocaleID
  // moved to LocaleModelHelp.chpl


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
    proc LocaleModel() {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      init();
    }

    proc LocaleModel(parent_loc : locale) {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      parent = parent_loc;
      init();
    }

    proc chpl_id() return _node_id; // top-level node number
    proc chpl_localeid() {
      return chpl_buildLocaleID(_node_id:chpl_nodeID_t, c_sublocid_any);
    }
    proc chpl_name() return local_name;


    proc writeThis(f) {
      // Most classes will define it like this:
//      f <~> name;
      // but here it is defined thus for backward compatibility.
      f <~> new ioLiteral("LOCALE") <~> _node_id;
    }

    proc getChildCount() return 0;

    proc getChild(idx:int) : locale {
      if boundsChecking then
        halt("requesting a child from a flat LocaleModel locale");
      return nil;
    }

    //------------------------------------------------------------------------{
    //- Implementation (private)
    //-
    proc init() {
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

    proc RootLocale() {
      parent = nil;
      nPUsPhysAcc = 0;
      nPUsPhysAll = 0;
      nPUsLogAcc = 0;
      nPUsLogAll = 0;
      maxTaskPar = 0;
    }

    // The init() function must use chpl_initOnLocales() to iterate (in
    // parallel) over the locales to set up the LocaleModel object.
    // In addition, the initial 'here' must be set.
    proc init() {
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

    proc getChild(idx:int) return this.myLocales[idx];

    proc getDefaultLocaleSpace() return this.myLocaleSpace;
    proc getDefaultLocaleArray() return myLocales;

    proc localeIDtoLocale(id : chpl_localeID_t) {
      // In the default architecture, there are only nodes and no sublocales.
      // What is more, the nodeID portion of a wide pointer is the same as
      // the index into myLocales that yields the locale representing that
      // node.
      return myLocales[chpl_rt_nodeFromLocaleID(id)];
    }
  }

  //////////////////////////////////////////
  //
  // utilities
  //
  inline
  proc chpl_getSubloc() {
    compilerError("must not call chpl_getSubloc() ",
                  "when locale model lacks sublocales");
    return c_sublocid_none;
  }

  //////////////////////////////////////////
  //
  // support for memory management
  //

  // chpl_here_alloc
  // chpl_here_calloc
  // chpl_here_realloc
  // chpl_here_good_alloc_size
  // chpl_here_free

  // moved to LocaleModelHelp.chpl


  //////////////////////////////////////////
  //
  // support for "on" statements
  //

  // chpl_executeOn
  // chpl_executeOnFast
  // chpl_executeOnNB

  // moved to LocaleModelHelpFlat.chpl


  //////////////////////////////////////////
  //
  // support for tasking statements: begin, cobegin, coforall
  //

  // chpl_taskListAddBegin
  // chpl_taskListAddCoStmt
  // chpl_taskListExecute

  // moved to LocaleModelHelp.chpl

}
