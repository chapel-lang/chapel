use GPU;
use CTypes;
use GpuDiagnostics;

proc doit() {
  var x = 100;
  writeln("CPU  BEFORE ALL: ", x);
  foreach i in 0..3 with (in x) {
    writeln("CPU  BEFORE: ", x);
    x = i;
    writeln("CPU  AFTER: ", x);
  }
  writeln("CPU  AFTER ALL: ", x);

  writeln();

  on here.gpus[0] {
    var x = 100;
    writeln("GPU  BEFORE ALL: ", x);
    @assertOnGpu
    foreach i in 0..3 with (in x) {
      gpuWritef("GPU  BEFORE: %i\n":c_ptrConst(c_char), x);
      x = i;
      gpuWritef("GPU  AFTER: %i\n":c_ptrConst(c_char), x);
    }
    writeln("GPU  AFTER ALL: ", x);
  }
}

doit();
