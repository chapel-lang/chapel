use GpuDiagnostics;

config const n = 10;

var cpuData1: [1..n] int;
var cpuData2: [1..n] int;

proc foo(ref x: int) { x += 1; }
proc bar(ref x: int) { x -= 1; }

var data2Copied: sync bool;


on here.gpus[0] {
  var gpuData1: [1..n] int;
  var gpuData2: [1..n] int;

  gpuData1 = cpuData1;

  startGpuDiagnostics();
  { // slide is more or less this block
    begin with (ref gpuData2) { // intent was not in slide by mistake
      gpuData2 = cpuData2;
      data2Copied.writeEF(true); }

    foreach d in gpuData1 do foo(d);

    if data2Copied.readFE() then

      foreach d in gpuData2 do bar(d);
  }
  stopGpuDiagnostics();

  cpuData1 = gpuData1;
  cpuData2 = gpuData2;
}

writeln(cpuData1);
writeln(cpuData2);

assertGpuDiags(kernel_launch=2);
