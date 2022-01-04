record R {
	var a : [1..10] int;
}
var d : domain(R);
d += new R();

writeln(d);
