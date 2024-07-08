proc main {
  use MemAlloc, MemDiagnostics;
  var m1, m2: uint(64);
  var o: opaque;
  m1 = memoryUsed();
  chpl_mem_free(o, -1, 0);
  m2 = memoryUsed();
  writeln(m2-m1);
}

