use GPU;

on here.gpus[0] {
  var Arr: [1..10] string;

  writeln(gpuSumReduce(Arr));
}
