use Time;
use GpuDiagnostics;

extern proc chpl_task_getRequestedSubloc(): int(32);

config const n = 1000;
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

startVerboseGpu();
startTest();
var CpuArr: [1..n] int;
stopTest("Cpu Array Init");

on here.gpus[0] {
  writeln(chpl_task_getRequestedSubloc());


  startTest();
  var GpuArr: [1..n] int;
  stopTest("Gpu Array Init");

  if touchOnGpu {
    startGpuDiagnostics();
    GpuArr = 1;
    stopGpuDiagnostics();
    assert(getGpuDiagnostics()[0].kernel_launch == 1);
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
stopVerboseGpu();

writeln(CpuArr.first);
