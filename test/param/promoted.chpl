// Test what happens when we promote a param or type function.

config param runParam = false;
config param runType = false;

const rng = 1..3;

proc valueFun(arg:int) return 777;
proc paramFun(arg:int) param return 888;
proc typeFun(arg:int) type return int;

var arr = valueFun(rng);
writeln(arr);

if runParam then
  param P = paramFun(rng);

if runType then
  type T =  typeFun(rng);
