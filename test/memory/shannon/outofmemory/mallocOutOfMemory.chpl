extern proc chpl_mem_allocMany(number, size, description, lineno = -1, filename=0): opaque;

for 1..1000 {
  var i = 1000000;
  chpl_mem_allocMany(i, numBytes(int(64)), 0, 5, __primitive("_get_user_file"));
}
halt("If everything works as expected, we should have run out of memory first");
