proc main {
  use MemAlloc, MemDiagnostics;
  var m1, m2, m3:uint(64);
  m1 = memoryUsed();
  var o: opaque = chpl_mem_allocMany(1, 100, 0, 0, 0);
  m2 = memoryUsed();
  chpl_mem_realloc(o, 0, 0, 0, 0);
  m3 = memoryUsed();

  writeln((m2-m1, m3-m1));
}
