class C { c:; };
class D { d:; };
makeC(a) : { c: new C; c.c = a; c };
makeD(a) : { d: new D; d.d = a; d };
c1 : makeC(1);
d1 : makeD(2);
d3 : makeD(c1);
print(d3.d.c + d1.d);
