
use CTypes;

// try with size = 2*alignment
for i in 3..20 {
  const alignment = (1 << i) : uint;
  var ptr = allocate(uint(8), (2*alignment).safeCast(c_size_t),
                     alignment=alignment.safeCast(c_size_t));
  writeln(alignment, " => ", ptr : c_uintptr : uint % alignment);
  deallocate(ptr);
}

// try with size = 1
for i in 3..20 {
  const alignment = (1 << i) : uint;
  var ptr = allocate(uint(8), 1, alignment=alignment.safeCast(c_size_t));
  writeln(alignment, " => ", ptr : c_uintptr : uint % alignment);
  deallocate(ptr);
}
