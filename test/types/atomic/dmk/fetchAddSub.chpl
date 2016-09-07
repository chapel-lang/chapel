var n: atomic int;
var x: atomic real;

n.write(42);
x.write(3.14);

writeln("fetchAdd:");
writeln("  old values = ", n, ", ", x);
writeln("  returned values (should equal old values) = ",
	n.fetchAdd(47), ", ", x.fetchAdd(3.14));
writeln("  new values = ", n, ", ", x);

writeln("fetchSub:");
writeln("  old values = ", n, ", ", x);
writeln("  returned values (should equal old values) = ",
	n.fetchSub(47), ", ", x.fetchSub(3.14));
writeln("  new values = ", n, ", ", x);
