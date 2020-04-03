var t = (3, );
writeln(t);

var (x) = t;
writeln(x);

proc f((x)) {
  writeln(x);
}
f(t);

var t1 = (1, 2);
var t2 = (3, 4);
var tt = ((...t1), (...t2));
writeln(tt);

param pu: uint = 2;
writeln(tt(pu));

var vu: uint = 2;
writeln(tt(vu));

var ttt = (1, 2.0, 3.0i);
writeln(ttt(pu));
