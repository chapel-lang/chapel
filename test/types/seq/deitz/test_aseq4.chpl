proc ii(i: int) return i;

var as1 = 1..10;
var bs = as1 by -2;
var cs = bs by -2;

writeln(as1, " = ", ii(as1));
writeln(bs, " = ", ii(bs));
writeln(cs, " = ", ii(cs));
