enum E {a=1, b=0, c=2};

var D: domain(E) = E.a..E.c;

writeln(D.sorted());

D -= E.a;
writeln(D.sorted());

D -= E.b;
writeln(D.sorted());

D += E.a;
writeln(D.sorted());

D += E.b;
writeln(D.sorted());

D.clear();
writeln(D.sorted());
