@gpu.assertEligible
var A = foreach i in 1..100 do i;

writeln("all is good; '@gpu.assertEligible' doesn't require GPU execution.");
