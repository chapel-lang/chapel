proc max(a:int, b:int): int{
	return (a > b) ?  a : b;
}

var a: int = 9;
var b: int = 10;

writeln("Max of ", a, ", ", b, " is: ", max(a,b));
