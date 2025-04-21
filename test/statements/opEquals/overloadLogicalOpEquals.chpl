operator &&=(ref lhs: bool, rhs: bool) do lhs = false;
operator ||=(ref lhs: bool, rhs: bool) do lhs = false;

var b = true;
var c = false;
b &&= c;
b ||= c;

writeln(b);
