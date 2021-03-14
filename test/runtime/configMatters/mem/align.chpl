use CPtr;
use SysCTypes;

// try with size = 2*alignment
for i in 3..20 {
  const alignment = (1 << i) : uint;
  var ptr = c_aligned_alloc(uint(8), alignment, 2*alignment);
  writeln(alignment, " => ", ptr : c_uintptr : uint % alignment);
  c_free(ptr);
}

// try with size = 1
for i in 3..20 {
  const alignment = (1 << i) : uint;
  var ptr = c_aligned_alloc(uint(8), alignment, 1);
  writeln(alignment, " => ", ptr : c_uintptr : uint % alignment);
  c_free(ptr);
}
