use CTypes;


extern proc chpl_gpu_get_alloc_size(arg): size_t;
extern proc chpl_gpu_copy_device_to_host(dst: c_void_ptr, src: c_void_ptr, n): void;
extern proc chpl_gpu_copy_host_to_device(dst: c_void_ptr, src: c_void_ptr, n): void;
extern proc chpl_gpu_is_device_ptr(ptr): bool;

config const n = 3;

var ptrHst = chpl_here_alloc(n, 0): c_ptr(c_uchar);

for i in 0..#n do
  ptrHst[i] = (i*10):uint(8);

var ptrHstResult = chpl_here_alloc(n, 0): c_ptr(c_uchar);

var expandedPtrHst = chpl_here_alloc(n*2, 0): c_ptr(c_uchar);

var s: uint;
on here.getChild(1) {

  ////////////////////////////////////////////////////

  writeln();
  writeln("Start testing chpl_here_alloc");

  var ptrDev = chpl_here_alloc(n, 0): c_ptr(c_uchar);
  writeln("Allocated ", chpl_gpu_get_alloc_size(ptrDev), " bytes on gpu");

  chpl_gpu_copy_host_to_device(ptrDev, ptrHst, n);

  writeln("Data copied to device");

  chpl_gpu_copy_device_to_host(ptrHstResult, ptrDev, n);

  writeln("Data copied back to host:");
  for i in 0..#n do
    write(ptrHstResult[i], " ");
  writeln();

  writeln("End testing chpl_here_alloc");
  writeln();

  ////////////////////////////////////////////////////

  writeln();
  writeln("Start testing chpl_here_calloc");

  var ptrDevZeroes = chpl_here_calloc(n, 1, 0): c_ptr(c_uchar);
  writeln("Allocated ", chpl_gpu_get_alloc_size(ptrDevZeroes), " bytes on gpu");

  chpl_gpu_copy_device_to_host(ptrHstResult, ptrDevZeroes, n);

  writeln("Data copied to host:");
  for i in 0..#n do
    write(ptrHstResult[i], " ");
  writeln();

  writeln("End testing chpl_here_calloc");
  writeln();

  ////////////////////////////////////////////////////

  writeln();
  writeln("Start testing chpl_here_realloc");

  var expandedPtrDev = chpl_here_realloc(ptrDev, n*2, 0);
  writeln("Allocated ", chpl_gpu_get_alloc_size(expandedPtrDev), " bytes on gpu");

  chpl_gpu_copy_host_to_device((expandedPtrDev:c_ptr(c_uchar)+n):c_void_ptr, ptrHst, n);

  writeln("Data copied to device");

  chpl_gpu_copy_device_to_host(expandedPtrHst, expandedPtrDev, n*2);

  writeln("Data copied back to host");
  for i in 0..#(n*2) do
    write(expandedPtrHst[i], " ");
  writeln();


  writeln("End testing chpl_here_realloc");
  writeln();

  ////////////////////////////////////////////////////

  writeln();
  writeln("Start testing chpl_here_free");

  // ptrDev has already been freed by realloc
  writeln("Should be false: ", chpl_gpu_is_device_ptr(ptrDev));

  writeln("Should be true: ", chpl_gpu_is_device_ptr(ptrDevZeroes));
  writeln("Should be true: ", chpl_gpu_is_device_ptr(expandedPtrDev));

  chpl_here_free(ptrDevZeroes);
  chpl_here_free(expandedPtrDev);

  writeln("End testing chpl_here_free");
  writeln();

}

writeln("End of test");
