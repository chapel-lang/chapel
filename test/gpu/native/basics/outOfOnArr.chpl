use GpuDiagnostics;

config const n = 10;

class ArrWrapperClass {
  const Dom: domain(?);
  forwarding var Arr: [Dom] int;
}

type ArrWrapper = owned ArrWrapperClass?;

startGpuDiagnostics();

var HostArr = new ArrWrapper({1..n});
var DevArr: HostArr.type; // make sure to not use a generic type

on here.gpus[0] {
  DevArr = new ArrWrapper({1..n}); // allocates the array on the GPU

  foreach a in DevArr! do
    a += 1;
}

on here.gpus[0] {
  foreach a in DevArr! do
    a += 1;
}

HostArr!.Arr = DevArr!.Arr;

writeln(HostArr!.Arr);

stopGpuDiagnostics();
assertGpuDiags(kernel_launch_aod=3, kernel_launch_um=2, device_to_host=1);
