var t = (1,);

writeln("t is: ", t, " (type = ", t.type:string, ")");

writeln("(...t) is: ", (...t));

proc foo(x...) {
  writeln("In foo(), x is: ", x);
}

foo((...t));
var x: 1*int;

writeln("x is: ", x);
