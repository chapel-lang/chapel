var x_default : int = 2;
var y_default : int = 4;

function foo(x : int = x_default, y : int = y_default) : int return x + y;

writeln(foo());
