config param case = 0;

const loc = if here.gpus.size > 0 then here.gpus[0] else here;

on loc {
  if case == 0 {
    var A: [1..10] int;
    @gpu.assertEligible
    foreach i in 1..10 {
      A[i] = i*i;
    }
  } else if case == 1 {
    @gpu.assertEligible
    var A = foreach i in 1..10 do i*i;
  }
}
