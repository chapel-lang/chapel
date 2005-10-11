var x_default : integer = 2;
var y_default : integer = 4;

function foo(x : integer = x_default, y : integer = y_default) return x + y;

writeln(foo());
