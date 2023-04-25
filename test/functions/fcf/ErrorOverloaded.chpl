// At the moment one cannot capture overloaded procedures.
proc foo(x: int) do return 0;
proc foo(x: real) do return 0.0;
proc foo(x: string) do return "0";

var p = foo;
var x = p(0);
writeln(x);
