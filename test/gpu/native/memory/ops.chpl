// this test is similar to basic.chpl but for memory operations. Currently I am
// adding only memset, but we can think of testing memmove, and memcpy like
// operations here.

use CTypes;

extern type c_sublocid_t = int(32);

extern proc chpl_gpu_memset(addr: c_ptr(void), val, size);
extern proc chpl_gpu_copy_device_to_host(dst: c_ptr(void), src_dev: c_sublocid_t,
                                         src: c_ptr(void), n,
                                         commID=0, ln=0, fn=0): void;

config const n = 3;

var ptrHst = chpl_here_alloc(n, 0): c_ptr(c_uchar);

const deviceId = 0: c_sublocid_t;
on here.gpus[deviceId] {
  var ptrDev = chpl_here_alloc(n, 0): c_ptr(c_uchar);

  ////////////////////////////////////////////////////

  writeln();
  writeln("Start testing chpl_gpu_memset");

  chpl_gpu_memset(ptrDev, 1, n);

  chpl_gpu_copy_device_to_host(ptrHst, deviceId, ptrDev, n);

  for i in 0..<n do
    write(ptrHst[i], " ");
  writeln();

  writeln("End testing chpl_gpu_memset");
  writeln();

  ////////////////////////////////////////////////////
}
