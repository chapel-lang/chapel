enum E { a,b,c,d,e };

var D: domain(E) = E.a..E.e;
D -= E.e;
D -= E.d;
D += E.e;
D += E.d;

var A: [D] int = [E.a=>1,E.b=>2,E.c=>3,E.d=>4,E.e=>5];

for i in A.sorted() {
  writeln(i);
}

writeln(A.sorted());
writeln(D);

D.remove(E.c);

for e in D.sorted() {
  writeln((e, A(e)));
}

enum F { a=1, b, c, d=-3, e, f };
var D2: domain(F) = F.a..F.f;
 
for i in D2.sorted() {
  writeln(i);
}

writeln(D2);
writeln(D2.sorted());

var A2: [D2] int;
for (a,i) in zip(A2, D2) {
  a = chpl__enumToOrder(i) + 1;
}

writeln(A2);
writeln(A2.sorted());

for d in D2.sorted() {
  writeln((d, A2(d)));
}

for (a, d) in zip(A2, D2) {
  if a != A2(d) then
    writeln("Error A2(", d, ") = ", A2(d), " a = ", a);
}
