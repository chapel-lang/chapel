g (x:float) : x;
f (x:any) : g(x);
f (x:int) : x + 3;
print(f(1));
print(f(2.0));
