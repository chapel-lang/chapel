use GpuDiagnostics;

var A = foreach i in 1..10 do i;
// Allocate B on the GPU
on here.gpus[0] var B = A;
writeln(B.locale == here.gpus[0]);

// Increment it from a kernel (makes sure the memory is accessible)
on here.gpus[0] {
  @assertOnGpu
  foreach b in B do b += 1;
}

writeln(B);
