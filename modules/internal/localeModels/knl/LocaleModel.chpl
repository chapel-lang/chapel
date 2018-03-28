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

  use LocaleModelHelpNUMA;

  require "hbwmalloc.h", "-lmemkind";

  extern proc hbw_check_available():c_int;
  extern proc hbw_malloc(size:size_t):c_void_ptr;
  extern proc hbw_calloc(nmemb:size_t, size:size_t):c_void_ptr;
  extern proc hbw_realloc(ptr:c_void_ptr, size:size_t):c_void_ptr;
  extern proc hbw_free(ptr:c_void_ptr);


  /* Why is a memory a locale?

     * A locale is a representation of system resources.
     * The alternative ways of expressing that an allocation
       should go on a particular memory seem unfortunate.
     * It is desirable e.g. to Block distribute over
       a particular kind of memory across nodes in the system.
  */

  // The following must be functions instead of constants.  Due to
  // module initialization order, constants are not visible early
  // enough.

  // It would be nice if one day these actually
  // corresponded with memkind.h's memory kinds.

  // platform memory -- big, lower latency, lower bandwidth
  private inline proc memoryKindDDR() param return 0;

  // on-chip memory -- small, higher latency, higher bandwidth
  private inline proc memoryKindMCDRAM() param return 1;

  private inline proc numMemoryKinds() param return 2;


  //
  // The packing scheme preserves the NUMA node numbering for compute NUMAs.
  // Memory-only NUMA nodes have subloc IDs >= the number of compute NUMAs.
  //
  // e.g. on a 2 NUMA domain system:
  // subloc could be:
  //   < 0 (c_sublocid_*) for a symbolic NUMA domain
  //   0 for NUMA domain 0
  //   1 for NUMA domain 1
  // with 2 memory kinds:
  //   0 for DDR
  //   1 for HBM
  //
  // < 0 == packSublocID(2, c_sublocid_*,  0)
  //   0 == packSublocID(2, 0,  0)
  //   1 == packSublocID(2, 1,  0)
  //   2 == packSublocID(2, 0,  1)
  //   3 == packSublocID(2, 1,  1)
  //   4 == packSublocID(2, c_sublocid_any,  1)
  //

  private inline proc defaultMemoryKind() param return memoryKindDDR();

  private inline
  proc numaDomainForAny(nNumaDomains:int) {
    return numMemoryKinds()*nNumaDomains;
  }

  //
  // This works even if subloc is already packed.
  // In that case, subloc is adjusted to the new memory kind.
  //
  private inline
  proc packSublocID(nNumaDomains:int, subloc:int, memoryKind:int)
  {
    var whichNuma = subloc;
    if whichNuma == c_sublocid_none || whichNuma == c_sublocid_all then
      return whichNuma;

    if whichNuma == c_sublocid_any ||
      whichNuma == numaDomainForAny(nNumaDomains) {
      if memoryKind == memoryKindMCDRAM() then
        return numaDomainForAny(nNumaDomains);
      else
        return c_sublocid_any;
    }
    if whichNuma >= nNumaDomains then
      whichNuma -= nNumaDomains;
    return (whichNuma + memoryKind*nNumaDomains):chpl_sublocID_t;
  }

  //
  // This works even if sub_id is already an unpacked NUMA node number.
  //
  private inline
  proc unpackSublocID(nNumaDomains:int, sub_id:chpl_sublocID_t)
  {
    var whichNuma = sub_id:int;
    var memoryKind:int;

    if whichNuma < nNumaDomains then
      memoryKind = defaultMemoryKind();
    else {
      if whichNuma == numaDomainForAny(nNumaDomains) then
        whichNuma = c_sublocid_any;
      else
        whichNuma -= nNumaDomains;
      memoryKind = memoryKindMCDRAM();
    }

    return (whichNuma, memoryKind);
  }

  //
  // The task layer calls these to convert between full sublocales and
  // execution sublocales.  Full sublocales include the memory kind in
  // this locale model.
  //
  export
  proc chpl_localeModel_sublocToExecutionSubloc(full_subloc:chpl_sublocID_t)
  {
    extern proc chpl_task_getNumSublocales(): int(32);
    const (whichNuma, memoryKind) =
      unpackSublocID(chpl_task_getNumSublocales(),
                     full_subloc:chpl_sublocID_t);
    return whichNuma:chpl_sublocID_t;
  }

  export
  proc chpl_localeModel_sublocMerge(full_subloc:chpl_sublocID_t,
                           execution_subloc:chpl_sublocID_t): chpl_sublocID_t
  {
    extern proc chpl_task_getNumSublocales(): int(32);
    var nNumaDomains:int = chpl_task_getNumSublocales();
    var memoryKind:int;
    //
    // Strip the memory kind out of the full_subloc and attach it
    // to the execution_subloc.
    //
    if full_subloc >= nNumaDomains then
      memoryKind = memoryKindMCDRAM();
    else
      memoryKind = defaultMemoryKind();
    return packSublocID(nNumaDomains, execution_subloc, memoryKind);
  }

  // Memory (DDR or MCDRAM?)
  class MemoryLocale : AbstractLocaleModel {
    const sid: chpl_sublocID_t;
    const mlName: string; // note: locale provides `proc name`

    proc chpl_id() return parent.chpl_id(); // top-level node id
    proc chpl_localeid() {
      return chpl_buildLocaleID(parent.chpl_id():chpl_nodeID_t, sid);
    }
    proc chpl_name() return mlName;

    //
    // Support for different types of memory:
    // large, low latency, and high bandwidth
    //
    proc defaultMemory() : locale {
      return parent.defaultMemory();
    }

    proc largeMemory() : locale {
      return parent.largeMemory();
    }

    proc lowLatencyMemory() : locale {
      return parent.lowLatencyMemory();
    }

    proc highBandwidthMemory() : locale {
      return parent.highBandwidthMemory();
    }

    proc init() {
    }

    proc init(_sid, _parent) {
      extern proc chpl_task_getNumSublocales(): int(32);

      super.init(_parent);

      sid = _sid: chpl_sublocID_t;
      const (whichNuma, kind) =
        unpackSublocID(chpl_task_getNumSublocales(), sid);
      var kindstr:string;
      if kind == memoryKindDDR() then
        kindstr = "DDR";
      else if kind == memoryKindMCDRAM() then
        kindstr = "MCDRAM";
      mlName = kindstr+whichNuma;
    }

    proc writeThis(f) {
      parent.writeThis(f);
      f <~> '.'+mlName;
    }

    proc getChildCount(): int { return 0; }
    proc getChild(idx:int) : locale { return nil; }
  }

  //
  // The NUMA sublocale model
  //
  class NumaDomain : AbstractLocaleModel {
    const sid : chpl_sublocID_t;
    const ndName : string; // note: locale provides `proc name`
    var ddr : MemoryLocale; // should never be modified after first assignment
    var hbm : MemoryLocale; // should never be modified after first assignment

    proc chpl_id() return parent.chpl_id(); // top-level node id
    proc chpl_localeid() {
      return chpl_buildLocaleID(parent.chpl_id():chpl_nodeID_t, sid);
    }
    proc chpl_name() return ndName;

    //
    // Support for different types of memory:
    // large, low latency, and high bandwidth
    //
    proc defaultMemory() : locale {
      return ddr;
    }

    proc largeMemory() : locale {
      return ddr;
    }

    proc lowLatencyMemory() : locale {
      return ddr;
    }

    proc highBandwidthMemory() : locale {
      return hbm;
    }

    proc init() {
    }

    proc init(_sid, _parent) {
      extern proc chpl_task_getNumSublocales(): int(32);
      var numSublocales = chpl_task_getNumSublocales();

      super.init(_parent);

      sid = packSublocID(numSublocales, _sid, defaultMemoryKind())
              : chpl_sublocID_t;
      ndName = "ND"+_sid;

      this.complete();

      ddr = new MemoryLocale(sid, this);

      var hbm_available = (hbw_check_available() == 0);
      // hbw_check_available() == 0 -> HBM is available.

      // Set up the high-bandwidth memory sublocale
      var memoryKind = if hbm_available
                       then memoryKindMCDRAM()
                       else defaultMemoryKind();
      const hbm_id = packSublocID(numSublocales, _sid, memoryKind);
      extern proc chpl_task_setSubloc(chpl_sublocID_t);
      const origSubloc = chpl_task_getRequestedSubloc();
      chpl_task_setSubloc(hbm_id);
      hbm = new MemoryLocale(hbm_id, this);

      chpl_task_setSubloc(origSubloc);
    }

    proc deinit() {
      delete ddr;
      delete hbm;
    }

    proc writeThis(f) {
      parent.writeThis(f);
      f <~> '.'+ndName;
    }

    proc getChildCount(): int { return 0; }
    iter getChildIndices() : int {
      halt("No children to iterate over.");
      yield -1;
    }
    proc addChild(loc:locale) { halt("Cannot add children to this locale type."); }
    proc getChild(idx:int) : locale { return nil; }

    // This is commented out b/c it leads to an internal error during
    // the resolveIntents pass.  See
    // test/functions/iterators/sungeun/iterInClass.future
    //
    // iter getChildren() : locale {
    //  halt("No children to iterate over.");
    //  yield nil;
    // }
  }

  //
  // The node model
  //
  class LocaleModel : AbstractLocaleModel {
    const _node_id : int;
    var local_name : string; // should never be modified after first assignment
    var ddr : MemoryLocale; // should never be modified after first assignment
    var hbm : MemoryLocale; // should never be modified after first assignment

    var numSublocales: int; // should never be modified after first assignment
    var childSpace: domain(1);
    var childLocales: [childSpace] NumaDomain;

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

    proc chpl_id() return _node_id;     // top-level locale (node) number
    proc chpl_localeid() {
      return chpl_buildLocaleID(_node_id:chpl_nodeID_t, c_sublocid_any);
    }
    proc chpl_name() return local_name;

    //
    // Support for different types of memory:
    // large, low latency, and high bandwidth
    //
    proc defaultMemory() : locale {
      return ddr;
    }

    proc largeMemory() : locale {
      return ddr;
    }

    proc lowLatencyMemory() : locale {
      return ddr;
    }

    proc highBandwidthMemory() : locale {
      return hbm;
    }


    proc writeThis(f) {
      // Most classes will define it like this:
      //      f <~> name;
      // but here it is defined thus for backward compatibility.
      f <~> new ioLiteral("LOCALE") <~> _node_id;
    }

    proc getChildSpace() return childSpace;

    proc getChildCount() return numSublocales;

    iter getChildIndices() : int {
      for idx in childSpace do
        yield idx;
    }

    proc getChild(idx:int) : locale {
      const (whichNuma, memoryKind) =
        unpackSublocID(numSublocales, idx:chpl_sublocID_t);
      if boundsChecking then
        if (whichNuma < 0) || (whichNuma >= numSublocales) then
          halt("sublocale child index out of bounds (",idx,")");
      if memoryKind == memoryKindMCDRAM() then
        return childLocales[whichNuma].hbm;
      else
        return childLocales[whichNuma];
    }

    iter getChildren() : locale  {
      for loc in childLocales do
        yield loc;
    }

    proc getChildArray() {
      return childLocales;
    }

    //------------------------------------------------------------------------{
    //- Implementation (private)
    //-
    proc setup() {
      helpSetupLocaleNUMA(this, local_name, numSublocales);

      ddr = new MemoryLocale(c_sublocid_any, this);

      var hbm_available = (hbw_check_available() == 0);
      // hbw_check_available() == 0 -> HBM is available.

      // Set up the high-bandwidth memory sublocale
      var memoryKind = if hbm_available
                       then memoryKindMCDRAM()
                       else defaultMemoryKind();
      const hbm_id = packSublocID(numSublocales, c_sublocid_any, memoryKind);
      const origSubloc = chpl_task_getRequestedSubloc();
      extern proc chpl_task_setSubloc(chpl_sublocID_t);
      chpl_task_setSubloc(hbm_id);
      hbm = new MemoryLocale(hbm_id, this);
      chpl_task_setSubloc(origSubloc);
    }
    //------------------------------------------------------------------------}

    proc deinit() {
      for loc in childLocales do
        delete loc;
      delete ddr;
      delete hbm;
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
      helpSetupRootLocaleNUMA(this);
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
      const node = chpl_nodeFromLocaleID(id);
      const subloc = chpl_sublocFromLocaleID(id);
      if subloc == numaDomainForAny(
                    (myLocales[node:int]:LocaleModel).numSublocales) then
        return ((myLocales[node:int]:LocaleModel).hbm):locale;
      else if chpl_isActualSublocID(subloc) then
        return (myLocales[node:int].getChild(subloc:int)):locale;
      else
        return (myLocales[node:int]):locale;
    }

    proc deinit() {
      for loc in myLocales {
        on loc {
          rootLocaleInitialized = false;
          delete loc;
        }
      }
    }
  }

  //////////////////////////////////////////
  //
  // support for memory management
  //

  //
  // We cannot use "here" here, due to order of declaration issues.
  //
  private inline
  proc allocatingInHbmSublocale(): bool {
    extern proc chpl_task_getNumSublocales(): int(32);
    return chpl_task_getRequestedSubloc() >= chpl_task_getNumSublocales();
  }

  private inline
  proc addrIsInHbm(addr:c_void_ptr): bool {
    extern proc chpl_topo_getMemLocality(p:c_void_ptr): chpl_sublocID_t;
    extern proc chpl_task_getNumSublocales(): int(32);
    return chpl_topo_getMemLocality(addr) >= chpl_task_getNumSublocales();
  }

  export
  proc chpl_addrIsInHbm(addr:c_void_ptr): bool {
    return addrIsInHbm(addr);
  }

  private extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;

  // The allocator pragma is used by scalar replacement.
  pragma "allocator"
  pragma "locale model alloc"
  proc chpl_here_alloc(size:int(64), md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_alloc(size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    if allocatingInHbmSublocale() then
      return hbw_malloc(size.safeCast(size_t));
    else
      return chpl_mem_alloc(size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_alloc(size:integral, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_alloc(size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    if allocatingInHbmSublocale() then
      return hbw_malloc(size.safeCast(size_t));
    else
      return chpl_mem_alloc(size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_calloc(size:integral, number:int, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_calloc(number:size_t, size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    if allocatingInHbmSublocale() then
      return hbw_calloc(number.safeCast(size_t), size.safeCast(size_t));
    else
      return chpl_mem_calloc(number.safeCast(size_t), size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_realloc(ptr:c_void_ptr, size:integral, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_realloc(ptr:c_void_ptr, size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    const useHbm = if ptr == nil
                   then allocatingInHbmSublocale()
                   else addrIsInHbm(ptr);
    if useHbm then
      return hbw_realloc(ptr, size.safeCast(size_t));
    else
      return chpl_mem_realloc(ptr, size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  proc chpl_here_good_alloc_size(min_size:integral): int {
    pragma "insert line file info"
      extern proc chpl_mem_good_alloc_size(min_size:size_t) : size_t;
    // memkind doesn't seem to provide one of these, so we'll
    // just get the # from the default allocator and hope for the best.
    // That's not totally crazy since they both might use jemalloc.
    return chpl_mem_good_alloc_size(min_size.safeCast(size_t)).safeCast(int);
  }

  pragma "locale model free"
  proc chpl_here_free(ptr:c_void_ptr): void {
    if ptr == nil then
      return;
    pragma "insert line file info"
      extern proc chpl_mem_free(ptr:c_void_ptr) : void;
    if addrIsInHbm(ptr) then
      hbw_free(ptr);
    else
      chpl_mem_free(ptr);
  }
}
