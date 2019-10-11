for i in 1..20 {
  const alignment = (1 << i) : uint;
  var ptr = c_aligned_alloc(uint(8), alignment, 2*alignment);
  writeln(alignment, " => ", ptr : uint % alignment);
  c_free(ptr);
}
