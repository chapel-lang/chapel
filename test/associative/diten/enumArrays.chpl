enum E { a,b,c,d,e };

var D: domain(E);
D -= E.e;
D -= E.d;
D += E.e;
D += E.d;

var A: [D] int = (1,2,3,4,5);

for i in A {
  writeln(i);
}

writeln(A);
writeln(D);

D.remove(E.c);

for e in D {
  writeln((e, A(e)));
}

enum F { a=1, b, c, d=-3, e, f };
var D2: domain(F);
 
for i in D2 {
  writeln(i);
}

writeln(D2);
writeln(D2.sorted());

var A2: [D2] int;
for (a,i) in zip(A2, 1..) {
  a = i;
}

writeln(A2);
writeln(A2.sorted());

for d in D2 {
  writeln((d, A2(d)));
}

for (a, d) in zip(A2, D2) {
  if a != A2(d) then
    writeln("Error A2(", d, ") = ", A2(d), " a = ", a);
}
