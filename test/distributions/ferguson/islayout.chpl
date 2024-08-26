use CTypes;
use LayoutCS;
use BlockDist;
use ExternalArray;

proc main() {
  // Default is "distributed" so it's not as hard
  // to keep up to date. But, check one to be sure.
  var blk = new blockDist(boundingBox={1..10});
  assert(!blk.dsiIsLayout());
  
  var mycs = new cs();
  assert(mycs.dsiIsLayout());

  // Check also the distribution for default arrays,
  // array slices, array reindexes, and array rank-changes.
  var A:[1..3, 1..3] int;

  // check DR
  assert(A.domain.distribution.dsiIsLayout());
  // check slice
  assert(A[1..3,1..2].domain.distribution.dsiIsLayout());
  // check rank-change
  assert(A[..,1].domain.distribution.dsiIsLayout());
  assert(A[1..3,1].domain.distribution.dsiIsLayout());
  // check reindex
  assert(A.reindex({0..2,0..2}).domain.distribution.dsiIsLayout());

  // check external array
  var ptr:c_ptr(int) = allocate(int, 1, clear=true);
  var B = makeArrayFromPtr(ptr, 1);
  assert(B.domain.distribution.dsiIsLayout());

  //make array from ptr creates an array that borrows the buffer. So, the
  //allocation needs to be cleaned up using the pointer.
  deallocate(ptr);
}
