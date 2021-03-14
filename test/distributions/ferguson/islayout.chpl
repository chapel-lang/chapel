use CPtr;
use LayoutCS;
use BlockDist;
use ExternalArray;

proc main() {
  // Default is "distributed" so it's not as hard
  // to keep up to date. But, check one to be sure.
  var blk = new unmanaged Block(boundingBox={1..10});
  assert(!blk.dsiIsLayout());
  
  //properly cleanup the distribution
  blk.dsiDestroyDist();
  delete blk;

  var cs = new unmanaged CS();
  assert(cs.dsiIsLayout());
  delete cs;

  // Check also the distribution for default arrays,
  // array slices, array reindexes, and array rank-changes.
  var A:[1..3, 1..3] int;

  // check DR
  assert(A.domain.dist.dsiIsLayout());
  // check slice
  assert(A[1..3,1..2].domain.dist.dsiIsLayout());
  // check rank-change
  assert(A[..,1].domain.dist.dsiIsLayout());
  assert(A[1..3,1].domain.dist.dsiIsLayout());
  // check reindex
  assert(A.reindex({0..2,0..2}).domain.dist.dsiIsLayout());

  // check external array
  var ptr:c_ptr(int) = c_calloc(int, 1);
  var B = makeArrayFromPtr(ptr, 1);
  assert(B.domain.dist.dsiIsLayout());

  //make array from ptr creates an array that borrows the buffer. So, the
  //allocation needs to be cleaned up using the pointer.
  c_free(ptr);
}
