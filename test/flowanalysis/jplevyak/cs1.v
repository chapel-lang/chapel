create x : { a: x; }; 
x : create 1; 
x.a = 3; 
print(x.a);
y : create 2.0; 
y.a = 4.0; 
print(y.a);
