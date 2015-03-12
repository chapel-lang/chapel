extern proc chpl_mem_allocMany(number, size, description, lineno = -1, filename: c_string = ""): opaque;

for 1..1000 {
  var i = 1000000;
  chpl_mem_allocMany(i, numBytes(int(64)), 0, 5, "mallocOutOfMemory.chpl");
}
halt("If everything works as expected, we should have run out of memory first");
