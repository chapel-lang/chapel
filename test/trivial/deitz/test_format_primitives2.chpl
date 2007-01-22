// Like test_format_primitives.chpl, but we use int(64) and real(32) instead.

var i : int(64) = 3;
writeln(i:"%lld");

var s : string;
s = i:"%lld";
writeln(s);


var f : real(32) = 1.23:real(32);
writeln(f:"%e");

var c : complex = 2.0 + 3.0i;
writeln(c:"(%g,%g)");

