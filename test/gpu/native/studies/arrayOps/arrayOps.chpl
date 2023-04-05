use Time;
use GpuDiagnostics;

config const n = 1000;
config const verboseGpu = false;
config const touchOnGpu = false;

var t: stopwatch;

proc startTest() {
  t.start();
}

proc stopTest(name) {
  t.stop();
  writeln(name, ": ", t.elapsed());
  t.clear();
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
  stopTest("host to device copy");

  if touchOnGpu {
    startGpuDiagnostics();
    GpuArr = 1;
    stopGpuDiagnostics();
    assert(getGpuDiagnostics()[0].kernel_launch == 1);
  }

  startTest();
  CpuArr = GpuArr;
  stopTest("device to host copy");

}
if verboseGpu then stopVerboseGpu();

writeln(CpuArr.first);
