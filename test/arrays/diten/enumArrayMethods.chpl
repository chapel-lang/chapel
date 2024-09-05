import Sort;
enum E {a=1, b=0, c=2};

var D: domain(E) = E.a..E.c;

writeln(Sort.sorted(D));

D -= E.a;
writeln(Sort.sorted(D));

D -= E.b;
writeln(Sort.sorted(D));

D += E.a;
writeln(Sort.sorted(D));

D += E.b;
writeln(Sort.sorted(D));

D.clear();
writeln(Sort.sorted(D));
