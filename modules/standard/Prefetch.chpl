/**
 *  Prefetch
 */
module Prefetch {
  inline proc prefetch(addr:c_ptr) {
    Prefetch_internal.chpl_prefetch(addr:c_void_ptr);
  }

  inline proc prefetch(addr:c_void_ptr) {
    Prefetch_internal.chpl_prefetch(addr);
  }
}

module Prefetch_internal {
  extern proc chpl_prefetch(addr: c_void_ptr);
}
