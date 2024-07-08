use GpuDiagnostics;

var gpuLocale = here.gpus[0];

var A = foreach i in 1..10 do i;
// Allocate B on the GPU
on gpuLocale var B = A;
writeln(B.locale == gpuLocale);
on B do writeln(here == gpuLocale);

// Increment it from a kernel (makes sure the memory is accessible)
on gpuLocale {
  @assertOnGpu
  foreach b in B do b += 1;
}

writeln(B);
