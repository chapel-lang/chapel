/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
@unstable("The GPU locale interface is unstable and expected to change in the forthcoming releases")
module LocaleModel {

  public use LocaleModelHelpGPU;

  use IO, CTypes;

  private inline
  proc runningOnGPUSublocale(): bool {
    return chpl_task_getRequestedSubloc()>=0;
  }

  private inline
  proc addrIsInGPU(addr:c_void_ptr): bool {
    extern proc chpl_gpu_is_device_ptr(ptr): bool;
    return chpl_gpu_is_device_ptr(addr);
  }

  pragma "fn synchronization free"
  private extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;

  // Note that there are 2 nearly identical chpl_here_alloc() functions. This
  // one takes an int(64) size and is marked with "locale model alloc" while
  // the second version takes a generic `integral` size and is not marked
  // "locale model alloc". Calls to the "locale model alloc" version are
  // inserted by the compiler (sometimes after resolution) for class/record
  // allocations. As a result, there can only be a single function with "locale
  // model alloc" in any compilation and the function must be fully specified.
  pragma "allocator"
  pragma "locale model alloc"
  pragma "always propagate line file info"
  proc chpl_here_alloc(size:int(64), md:chpl_mem_descInt_t): c_void_ptr {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_alloc(size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;

    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_gpu_mem_alloc(size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;


    if runningOnGPUSublocale() then
      return chpl_gpu_mem_alloc(size.safeCast(c_size_t), md + chpl_memhook_md_num());
    else
      return chpl_mem_alloc(size.safeCast(c_size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  pragma "llvm return noalias"
  pragma "always propagate line file info"
  proc chpl_here_alloc(size:integral, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_alloc(size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;

    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_gpu_mem_alloc(size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;


    if runningOnGPUSublocale() then
      return chpl_gpu_mem_alloc(size.safeCast(c_size_t), md + chpl_memhook_md_num());
    else
      return chpl_mem_alloc(size.safeCast(c_size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  pragma "llvm return noalias"
  pragma "always propagate line file info"
  proc chpl_here_aligned_alloc(alignment:integral, size:integral,
                               md:chpl_mem_descInt_t): c_void_ptr {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_memalign(alignment:c_size_t, size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;

    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_gpu_mem_memalign(alignment:c_size_t, size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;

    if runningOnGPUSublocale() then
      return chpl_gpu_mem_memalign(alignment.safeCast(c_size_t),
                                   size.safeCast(c_size_t),
                                   md + chpl_memhook_md_num());
    else
      return chpl_mem_memalign(alignment.safeCast(c_size_t),
                               size.safeCast(c_size_t),
                               md + chpl_memhook_md_num());
  }

  pragma "allocator"
  pragma "llvm return noalias"
  pragma "always propagate line file info"
  proc chpl_here_calloc(size:integral, number:integral, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_calloc(number:c_size_t, size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;

    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_gpu_mem_calloc(number:c_size_t, size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;

    if runningOnGPUSublocale() then
      return chpl_gpu_mem_calloc(number.safeCast(c_size_t), size.safeCast(c_size_t), md + chpl_memhook_md_num());
    else
      return chpl_mem_calloc(number.safeCast(c_size_t), size.safeCast(c_size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  pragma "always propagate line file info"
  proc chpl_here_realloc(ptr:c_void_ptr, size:integral, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_realloc(ptr:c_void_ptr, size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;

    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_gpu_mem_realloc(ptr:c_void_ptr, size:c_size_t, md:chpl_mem_descInt_t) : c_void_ptr;

    if addrIsInGPU(ptr) {
      if !runningOnGPUSublocale() {
        halt("Trying to realloc a GPU pointer outside a GPU sublocale");
      }
      return chpl_gpu_mem_realloc(ptr, size.safeCast(c_size_t), md + chpl_memhook_md_num());
    }
    else {
      return chpl_mem_realloc(ptr, size.safeCast(c_size_t), md + chpl_memhook_md_num());
    }
  }

  pragma "fn synchronization free"
  pragma "always propagate line file info"
  proc chpl_here_good_alloc_size(min_size:integral): min_size.type {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_good_alloc_size(min_size:c_size_t) : c_size_t;

    // This is currently here only for completeness: I am not sure if need
    // something like this for the GPU, and it doesn't call anything specific
    // for it.
    return chpl_mem_good_alloc_size(min_size.safeCast(c_size_t)).safeCast(min_size.type);
  }

  pragma "locale model free"
  pragma "always propagate line file info"
  proc chpl_here_free(ptr:c_void_ptr): void {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_free(ptr:c_void_ptr) : void;

    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_gpu_mem_free(ptr:c_void_ptr) : void;

    if addrIsInGPU(ptr) {
      if !runningOnGPUSublocale() {
        halt("Trying to free a GPU pointer outside a GPU sublocale");
      }
      chpl_gpu_mem_free(ptr);
    }
    else {
      chpl_mem_free(ptr);
    }
  }

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

  class GPULocale : AbstractLocaleModel {
    const sid: chpl_sublocID_t;

    override proc chpl_id() do return try! (parent._value:LocaleModel)._node_id; // top-level node id
    override proc chpl_localeid() {
      return try! chpl_buildLocaleID((parent._value:LocaleModel)._node_id:chpl_nodeID_t,
                                     sid);
    }
    override proc chpl_name() {
      return try! (parent._value:LocaleModel).local_name + "-GPU" + sid:string;
    }

    proc init() {
    }
    proc deinit() {
     extern proc hsa_shutdown(): void ;

     // Comment out this until runtime support for HSA is added

     //     hsa_shutdown();
    }


    proc init(_sid, _parent) {
      super.init(new locale(_parent));
      sid = _sid;
    }

    override proc writeThis(f) throws {
      parent.writeThis(f);
      f.write("-GPU" + sid:string);
    }

    override proc getChildCount(): int { return 0; }
    override proc _getChildCount(): int { return 0; }

    iter getChildIndices() : int {
      halt("No children to iterate over.");
      yield -1;
    }
    proc addChild(loc:locale) {
      halt("Cannot add children to this locale type.");
    }
    override proc getChild(idx:int) : locale {
      halt("requesting a child from a GPULocale locale");
      return new locale(this);
    }
    override proc _getChild(idx:int) : locale {
      halt("requesting a child from a GPULocale locale");
      return new locale(this);
    }

    override proc isGpu() : bool { return true; }
  }

  const chpl_emptyLocaleSpace: domain(1) = {1..0};
  pragma "unsafe"
  const chpl_emptyLocales: [chpl_emptyLocaleSpace] locale;

  //
  // A concrete class representing the nodes in this architecture.
  //
  class LocaleModel : AbstractLocaleModel {
    const _node_id : int;
    var local_name : string; // should never be modified after first assignment

    var numSublocales: int;
    const childSpace: domain(1);
    param firstGpuSubLocId = 0;

    pragma "unsafe"
    var childLocales: [childSpace] unmanaged AbstractLocaleModel;

    pragma "unsafe"
    var gpuSublocales: [childSpace] locale;

    // This constructor must be invoked "on" the node
    // that it is intended to represent.  This trick is used
    // to establish the equivalence the "locale" field of the locale object
    // and the node ID portion of any wide pointer referring to it.
    proc init() {
      if rootLocaleInitialized {
        halt("Cannot create additional LocaleModel instances");
      }
      _node_id = chpl_nodeID: int;

      extern proc chpl_gpu_get_device_count(ref n: int);
      var nDevices: int;
      chpl_gpu_get_device_count(nDevices);

      //number of GPU devices on a node
      numSublocales = nDevices;
      childSpace = {0..#numSublocales};

      this.complete();

      setup();
    }

    proc init(parent_loc : locale) {
      if rootLocaleInitialized {
        halt("Cannot create additional LocaleModel instances");
      }

      super.init(parent_loc);

      _node_id = chpl_nodeID: int;

      extern proc chpl_gpu_get_device_count(ref n: int);
      var nDevices: int;
      chpl_gpu_get_device_count(nDevices);

      //1 cpu and number of GPU devices on a node
      numSublocales = nDevices;
      childSpace = {0..#numSublocales};

      this.complete();

      setup();
    }

    // top-level locale (node) number
    override proc chpl_id() do return _node_id;

    override proc chpl_localeid() {
      return chpl_buildLocaleID(_node_id:chpl_nodeID_t, c_sublocid_none);
    }
    override proc chpl_name() do return local_name;

    proc getChildSpace() do return childSpace;

    override proc getChildCount() do return numSublocales;
    override proc _getChildCount() do return numSublocales;

    iter getChildIndices() : int {
      for idx in {0..#numSublocales} do // chpl_emptyLocaleSpace do
        yield idx;
    }

    override proc getChild(idx:int) : locale {
      if boundsChecking then
        if (idx < 0) || (idx >= numSublocales) then
          halt("sublocale child index out of bounds (",idx,")");
      return new locale(childLocales[idx]);
    }
    override proc _getChild(idx:int) : locale {
      if boundsChecking then
        if (idx < 0) || (idx >= numSublocales) then
          halt("sublocale child index out of bounds (",idx,")");
      return new locale(childLocales[idx]);
    }

    pragma "no doc"
    override proc gpusImpl() const ref {
      return gpuSublocales;
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
      helpSetupLocaleGPU(this, local_name, numSublocales, GPULocale);
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
      helpSetupRootLocaleGPU(this);
    }

    // Has to be globally unique and not equal to a node ID.
    // We return numLocales for now, since we expect nodes to be
    // numbered less than this.
    // -1 is used in the abstract locale class to specify an invalid node ID.
    override proc chpl_id() do return numLocales;
    override proc chpl_localeid() {
      return chpl_buildLocaleID(numLocales:chpl_nodeID_t, c_sublocid_none);
    }
    override proc chpl_name() do return local_name();
    proc local_name() do return "rootLocale";

    override proc writeThis(f) throws {
      f.write(name);
    }

    override proc getChildCount() do return this.myLocaleSpace.size;
    override proc _getChildCount() do return this.myLocaleSpace.size;

    proc getChildSpace() do return this.myLocaleSpace;

    iter getChildIndices() : int {
      for idx in this.myLocaleSpace do
        yield idx;
    }

    override proc getChild(idx:int) do return this.myLocales[idx];
    override proc _getChild(idx:int) do return this.myLocales[idx];

    iter getChildren() : locale  {
      for loc in this.myLocales do
        yield loc;
    }

    override proc getDefaultLocaleSpace() const ref do return this.myLocaleSpace;
    override proc getDefaultLocaleArray() const ref do return myLocales;

    override proc localeIDtoLocale(id : chpl_localeID_t) {
      const node = chpl_nodeFromLocaleID(id);
      const subloc = chpl_sublocFromLocaleID(id);
      if (subloc == c_sublocid_none) || (subloc == c_sublocid_any) then
        return (myLocales[node:int]):locale;
      else
        return (myLocales[node:int]._getChild(subloc:int)):locale;
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
