f(x:int) : x + 1;
g x : f x;
h x : g x;
print(h(1.0));
