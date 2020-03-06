use IO, Collection, Regexp;

param bp = b"brad";
var b = b"brad";
param sp = "brad";
var s = "brad";
param csp: c_string = c"brad";
var cs = c"brad";
var a: c_array(int, 20);
var r = 1..10;
const stdin = openfd(0);
var l = new LinkedList(int);
var re = "foobar".match(compile(".*oob.*"));
var c = new CollectionImpl(int);





writeln(bp.length);
writeln(b.length);
writeln(sp.length);
writeln(s.length);
writeln(csp.length);
writeln(c"brad".length);
writeln(cs.length);
writeln(a.length);
writeln(r.length);
writeln(stdin.length());
writeln(l.length);
writeln(re.length);
writeln(c.length);
