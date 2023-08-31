use GpuDiagnostics;

class Base {
  proc foo() { }
}

class Derived : Base {
  override proc foo() {
    var A : [0..10] int;
    foreach i in 0..10 {
      A[i] = i;
    }
  }
}

on here.gpus[0] {
  startGpuDiagnostics();

  var obj : Base = new Derived();
  obj.foo();

  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch_um=1, kernel_launch_aod=2);
}
