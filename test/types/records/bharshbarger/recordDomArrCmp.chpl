
record R {
	var space : domain(1); // still occurs if domain is initialized here
	var arr : [space] int;
}

var a : R;
var b : R;

writeln(a == b);
writeln(a != b);
