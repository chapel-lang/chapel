use Allocators;

var pool = new bumpPtrMemPool(10, parSafe=false);
var strTree = newWithAllocator(pool, unmanaged R, 2, pool);

class R {
  proc init(a, ref pool) { }
}
