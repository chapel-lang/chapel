// this test checks that implicit numeric conversions
// can apply for a 'const ref' formal argument in addition
// to 'const' or default intent.

proc fnConstRef(const ref arg: int) { writeln(arg); }
proc fnConst(const arg: int) { writeln(arg); }
proc fnBlank(arg: int) { writeln(arg); }

var x: int(16) = 12345;
fnConstRef(x);
fnConst(x);
fnBlank(x);
