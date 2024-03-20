pragma "no gpu codegen"
proc fNo(a: int, b: int) do return a * b;

config param promote = false;
config param useForall = false;

on here.gpus[0] {
  var captureMe: int = 1;

  if promote {
    if useForall {
      @assertOnGpu
      var A = fNo(forall i in 0..#128 do i, captureMe);
    } else {
      @assertOnGpu
      var A = fNo(foreach i in 0..#128 do i, captureMe);
    }
  } else {
    if useForall {
      @assertOnGpu
      var A = forall i in 0..#128 do fNo(i, i);
    } else {
      @assertOnGpu
      var A = foreach i in 0..#128 do fNo(i, i);
    }
  }
}
