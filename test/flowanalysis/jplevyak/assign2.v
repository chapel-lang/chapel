class C { c:; d:; };
c1 : new C;
f c2 : if (c1.c) return (c1.c) else return (c1.d); 
(f c1) = 1;
print(c1.c + c1.d);
