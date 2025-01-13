var CpuArr: [1..1_000_000] int;        // allocate an array on the host memory
on here.gpus[0] {                      // move the execution to the GPU
  var GpuArr = CpuArr;                 // copy the host array onto device memory
  forall elem in GpuArr do elem += 1;  // increment elements by 1
  CpuArr = GpuArr;                     // copy the array back to the host
}

// This was not part of the slide and only there for correctness
assert((+ reduce CpuArr) == 1_000_000);

