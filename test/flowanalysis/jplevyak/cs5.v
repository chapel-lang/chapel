class C { var c; };
class D { var d; };
makeC(var a) : { c : new C; c.c = a; c };
makeD(var a) : { d : new D; d.d = a; d };
c1 : makeC(1);
d1 : makeD(c1);
d3 : makeD(3.0);
print(d1.d.c + d3.d);
