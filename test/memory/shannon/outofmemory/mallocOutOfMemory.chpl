extern proc chpl_mem_allocMany(number, size, description, lineno = -1, filename = ""): opaque;

while (1) {
  var i = 1000000;
  chpl_mem_allocMany(i, numBytes(int(64)), 0, 5, "mallocOutOfMemory.chpl");
}
