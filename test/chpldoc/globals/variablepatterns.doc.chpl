var a = __primitive("get compiler variable", "CHPL_HOME");
var b = __primitive("get compiler variable", "chpl_lookupFilename", "CHPL_HOME");
var c = if true then 1 else 2;
var d = for i in 1..10 do i;
var e = forall i in 1..10 do i;
var f = [i in 1..10] i;
var g = try! x;
var h = try x;
