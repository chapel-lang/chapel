pragma "no gpu codegen"
proc fNo(a: int, b: int) do return a * b;

enum loopType {
  promotedOverLoopExpr,
  promotedOverArray,
  explicit
}
use loopType;

config param useLoopType = loopType.promotedOverLoopExpr;
config param useForall = false;

on here.gpus[0] {
  var captureMe: int = 1;

  select useLoopType {
    when promotedOverLoopExpr {
      if useForall {
        @assertOnGpu
        var A = fNo(forall i in 0..#128 do i, captureMe);
      } else {
        @assertOnGpu
        var A = fNo(foreach i in 0..#128 do i, captureMe);
      }
    }
    when promotedOverArray {
      var ArgArr: [1..128] int;

      if !useForall then
          compilerError("Can't use foreach loop when promoting over array");

      @assertOnGpu
      var A = fNo(ArgArr, captureMe);
    }
    when explicit {
      if useForall {
        @assertOnGpu
        var A = forall i in 0..#128 do fNo(i, i);
      } else {
        @assertOnGpu
        var A = foreach i in 0..#128 do fNo(i, i);
      }
    }
  }
}
