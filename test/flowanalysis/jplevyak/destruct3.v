class A { a: 1; b: 2.0;};
f() : new A;
(:A x:=b, y:=a) : f();
print(x + y);


