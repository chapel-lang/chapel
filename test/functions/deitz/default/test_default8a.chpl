var x_default : integer = 2;
var y_default : integer = 4;

function foo(x : integer = x_default, y : integer = y_default) : integer return x + y;

writeln(foo());

x_default = 10;

writeln(foo());
