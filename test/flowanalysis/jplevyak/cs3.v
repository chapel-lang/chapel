aa x : { a:x; }; 
bb x : { b:x; }; 
a1 : aa 1; 
b1 : bb a1;
a2 : aa 2.0; 
b2 : bb a2;
print b1.b.a + b2.b.a;
