for i in 1..20 {
  const alignment = (1 << i) : uint;
  var ptr = c_malloc(eltType=uint(8), size=1, alignment=alignment);
  writeln(alignment, " => ", ptr : uint % alignment);
  c_free(ptr);
}
