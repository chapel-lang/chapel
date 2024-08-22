use GpuDiagnostics;

on here.gpus[0] {
  var B: [1..10] bool;

  var res: bool;
  @assertOnGpu
  forall b in B with (&& reduce res) {
    res &&= b;
  }

  writeln(res);
}

