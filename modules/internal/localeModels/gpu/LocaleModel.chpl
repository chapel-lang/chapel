module LocaleModel {

  public use LocaleModelHelpFlat;
  public use LocaleModelHelpMem;

  use IO;

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
  

  //not in APU model but in flat model
  //const chpl_emptyLocaleSpace: domain(1) = {1..0};
  //pragma "unsafe"
  //const chpl_emptyLocales: [chpl_emptyLocaleSpace] locale;

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
      if (subloc == c_sublocid_none) || (subloc == c_sublocid_any) then
        return (myLocales[node:int]):locale;
      else
        return (myLocales[node:int].getChild(subloc:int)):locale;
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
