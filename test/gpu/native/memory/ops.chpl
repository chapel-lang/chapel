// this test is similar to basic.chpl but for memory operations. Currently I am
// adding only memset, but we can think of testing memmove, and memcpy like
// operations here.

use CTypes;

extern proc chpl_gpu_memset(addr: c_void_ptr, val, size);
extern proc chpl_gpu_copy_device_to_host(dst: c_void_ptr, src: c_void_ptr, n): void;

config const n = 3;

var ptrHst = chpl_here_alloc(n, 0): c_ptr(c_uchar);
on here.gpus[0] {
  var ptrDev = chpl_here_alloc(n, 0): c_ptr(c_uchar);

  ////////////////////////////////////////////////////

  writeln();
  writeln("Start testing chpl_gpu_memset");

  chpl_gpu_memset(ptrDev, 1, n);

  chpl_gpu_copy_device_to_host(ptrHst, ptrDev, n);

  for i in 0..<n do
    write(ptrHst[i], " ");
  writeln();

  writeln("End testing chpl_gpu_memset");
  writeln();

  ////////////////////////////////////////////////////
}
