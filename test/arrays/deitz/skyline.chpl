writeln([j in 1..3] [i in 1..j] i);

var A = for j in 1..3 do [i in 1..j] i;

writeln(A.domain);
writeln("  ", A(1).domain, "  ", A(1));
writeln("  ", A(2).domain, "  ", A(2));
writeln("  ", A(3).domain, "  ", A(3));
