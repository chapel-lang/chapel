def f(x: (?u,int)) { writeln("(?,int): ", x); }
def f(x: (?v,real)) { writeln("(?,real): ", x); }

f((1, 2));
f((1.0, 2));
f((1.0, 2.0));
f((1, 2.0));
