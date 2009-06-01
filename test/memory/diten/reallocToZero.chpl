def main {
  use MemAlloc, Memory;
  var m1, m2, m3:uint(64);
  m1 = memoryUsed();
  var o: opaque = chpl_malloc(1, 100, 0, 0, "");
  m2 = memoryUsed();
  chpl_realloc(o, 0, 0, 0);
  m3 = memoryUsed();

  writeln((m1, m2, m3));
}
