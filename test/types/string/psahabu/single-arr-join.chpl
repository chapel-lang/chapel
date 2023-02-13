var SA: [1..3 by 2  align 2] string = ["align 2", ];
var SB: [1..3 by -2 align 2] string = ["reverse iter", ];  // neg-stride

writeln(" ".join(SA));
writeln(" ".join(SB));
