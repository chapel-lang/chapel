f() : (1,2.0);
c : ((a, b) : f());
print(a + b);
print(c);

g() : (1,2,(3,4,(5, 6)));
(s,t,(u,v,(w,x))) : g();
print(s+t+u+v+w+x);
