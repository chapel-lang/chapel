use GPU;
use CTypes;

proc foo(ref x) {
  x = nil;
}

on here.gpus[0] {
  var x: c_ptr(int);
  foreach i in 1..10 {
    assertOnGpu();
    if i==1 then
      foo(x);
  }

  writeln(x);
}
