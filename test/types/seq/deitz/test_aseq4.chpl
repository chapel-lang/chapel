proc ii(i: int) return i;

var as = 1..10;
var bs = as by -2;
var cs = bs by -2;

writeln(as, " = ", ii(as));
writeln(bs, " = ", ii(bs));
writeln(cs, " = ", ii(cs));
