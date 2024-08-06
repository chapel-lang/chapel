use GpuDiagnostics;

on here.gpus[0] {
  var Arr  : [0..<2, 0..<2] int;
  var Arr2 : [0..<2, 0..<2] int;

  for i in 0..<4 do Arr[i/2, i%2] = i;
  writeln(Arr, "\n");

  startGpuDiagnostics();
  for x in Arr do Arr2[x/2, x%2] = x;
  stopGpuDiagnostics();

  writeln(Arr2);
}

assertGpuDiags(kernel_launch=0);
