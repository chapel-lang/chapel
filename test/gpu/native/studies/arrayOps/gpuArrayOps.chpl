use Time;
use GpuDiagnostics;

config const n = 1000;
config const verboseGpu = false;
config const touchOnGpu = false;
config const reportTime = true;

var t: stopwatch;

proc startTest() {
  if reportTime then t.start();
}

proc stopTest(name, bandwidth=false) {
  if reportTime {
    t.stop();
    if bandwidth then
      writeln(name, " (GB/s): ", n*numBytes(int)/t.elapsed()/1e9);
    else
      writeln(name, " (s): ", t.elapsed());
    t.clear();
  }
}

if verboseGpu then startVerboseGpu();
startTest();
var CpuArr: [1..n] int;
stopTest("Cpu Array Init");

on here.gpus[0] {

  startTest();
  var GpuArr: [1..n] int;
  stopTest("Gpu Array Init");

  if touchOnGpu {
    startGpuDiagnostics();
    GpuArr = 1;
    stopGpuDiagnostics();
    assert(getGpuDiagnostics()[0].kernel_launch == 1);
    resetGpuDiagnostics();
  }

  startTest();
  GpuArr = CpuArr;
  stopTest("host to device copy", bandwidth=true);

  if touchOnGpu {
    startGpuDiagnostics();
    GpuArr = 1;
    stopGpuDiagnostics();
    assert(getGpuDiagnostics()[0].kernel_launch == 1);
  }

  startTest();
  CpuArr = GpuArr;
  stopTest("device to host copy", bandwidth=true);


}
if verboseGpu then stopVerboseGpu();

assert(CpuArr.first == if touchOnGpu then 1 else 0);
