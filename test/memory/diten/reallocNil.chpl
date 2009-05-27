def main {
  use MemAlloc, Memory;
  var m1, m2, m3: uint(64);
  var o: opaque;
  m1 = memoryUsed();
  o = chpl_realloc(o, 100, 1, "realloc nil", -1, "");
  m2 = memoryUsed();
  chpl_free(o, -1, "");
  m3 = memoryUsed();
  writeln((m1, m2, m3));
}
