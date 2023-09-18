proc main {
  use MemAlloc, MemDiagnostics;
  var m1, m2, m3: uint(64);
  var o: opaque;
  m1 = memoryUsed();
  o = chpl_mem_realloc(o, 100, 0, -1, 0);
  m2 = memoryUsed();
  chpl_mem_free(o, -1, 0);
  m3 = memoryUsed();
  writeln((m2-m1, m3-m1));
}
