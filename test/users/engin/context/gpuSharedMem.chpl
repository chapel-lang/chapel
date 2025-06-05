use CTypes;
use GPU;

use ChapelContextSupport;
use Iterators.SimpleOneDim;

config param N=16;
config param BLOCK_SIZE=32;

config const n = 20;

on here.gpus[0] {
  var A : [0..<N] uint;

  @assertOnGpu
  foreach i in simpleOneDim(N) {  // context should be coming from a new syntax
    const context = new Context();
    const vectorContext = __primitive("outer context", context);

    // Each thread independently assigns to shared memory
    var dst_ptr: c_array(uint, N);
    __primitive("hoist to context", vectorContext, dst_ptr);

    // After the sync the array in shared memory will be:
    //   [100, 200, 300, 400, ...]
    dst_ptr[i] = ((i+1) * 100) : uint;
    syncThreads();

    // Grab value in shared memory assigned by neighbor to your right and add
    // your id
    A[i] = dst_ptr[(i + 1) % N] + i : uint;
  }

  // Print array, chunked by blocks. Note that each element is assigned by a
  // thread to the value stored in shared memory by the thread to its right
  // plus the thread id for the element itself. The exception is elements on
  // the right "border" of each block since it will be accessing an area of
  // shared memory that was not set by the block. 
  write(" A = [");
  for i in 0..<N {
    if i > 0 && i % BLOCK_SIZE == 0 {
      write(" | ");
    } else if i > 0 {
      write(" ");
    }
    write(A[i]);
  }
  writeln("]");

  // Print stable values:
  write(" A (stable) = [");
  for i in 0..<N {
    if i > 0 then write(" ");
    if i % BLOCK_SIZE != BLOCK_SIZE-1 {
      write(A[i]);
    }
  }
  writeln("]");
}
