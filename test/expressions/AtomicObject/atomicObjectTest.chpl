class O {
	var x : int;
}
var o : atomic O;
o.write(new O(1));
writeln(o.read());

