pragma "no use ChapelStandard"
module AtomicsCommon {
  record atomic_refcnt {
    // The common case seems to be local access to this class, so we
    // will use explicit processor atomics, even when network
    // atomics are available. In the future, we could have both
    // network and processor atomic versions of atomic_refcnt if
    // necessary.
    var _cnt:atomic_int64;
    // Reference counting implemented according to 
// http://www.chaoticmind.net/~hcb/projects/boost.atomic/doc/atomic/usage_examples.html#boost_atomic.usage_examples.example_reference_counters
// http://stackoverflow.com/questions/10268737/c11-atomics-and-intrusive-shared-pointer-reference-count
    inline proc inc(cnt=1) {
      // Note CHPL_CACHE_REMOTE activates more complicated behavior
      // only when the cache for remote data is enabled; it's possible
      // that the more complicated behavior would benefit single-locale
      // situations as well but curently our atomics implementation doesn't
      // do anything with the order argument (except for when the cache
      // for remote data is enabled).
      if CHPL_CACHE_REMOTE then _cnt.add(cnt, order=memory_order_relaxed);
      else _cnt.add(cnt);
    }
    // Returns the number of remaining references
    // (ie you are probably responsible for deleting the object if
    //  release returns 0)
    inline proc dec():int(64) {
      var got:int(64);
      // See comment in addref about use of CHPL_CACHE_REMOTE here.
      if CHPL_CACHE_REMOTE {
        got = _cnt.fetchSub(1, order=memory_order_release);
        if got == 1 {
          atomic_fence(memory_order_acquire);
          return 0;
        }
        return got - 1;
      } else {
        got = _cnt.fetchSub(1);
        return got - 1;
      }
    }
  }
}

