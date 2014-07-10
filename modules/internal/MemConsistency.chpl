pragma "no use ChapelStandard"
module MemConsistency {
 
  pragma "memory order type"
  extern type memory_order;

  extern const memory_order_relaxed:memory_order;
  extern const memory_order_consume:memory_order;
  extern const memory_order_acquire:memory_order;
  extern const memory_order_release:memory_order;
  extern const memory_order_acq_rel:memory_order;
  extern const memory_order_seq_cst:memory_order;

  // These functions are memory consistency fences (ie acquire or
  // release fences) for the remote data cache.
  pragma "insert line file info"
  extern proc chpl_rmem_consist_release();
  pragma "insert line file info"
  extern proc chpl_rmem_consist_acquire();
  pragma "insert line file info"
  extern proc chpl_rmem_consist_maybe_release(order:memory_order);
  pragma "insert line file info"
  extern proc chpl_rmem_consist_maybe_acquire(order:memory_order);
  pragma "insert line file info"
  extern proc chpl_rmem_consist_fence(order:memory_order);

  // Local memory consistency is handled in Atomics.chpl
  // and can be done from C.

}
