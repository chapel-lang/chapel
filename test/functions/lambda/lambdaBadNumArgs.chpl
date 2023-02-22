var compose = lambda(f: func(int,int), g: func(int,int)): func(int,int) {
    var res = lambda(x:int): int { return g(f(x)); };
    return res;
};

var inc = lambda(x: int): int { return x + 1; };

var inc2 = compose(inc, inc);

writeln(inc2(0));
