proc f(x: (?u,int)) { writeln("(?,int): ", x); }
proc f(x: (?v,real)) { writeln("(?,real): ", x); }

f((1, 2));
f((1.0, 2));
f((1.0, 2.0));
f((1, 2.0));
