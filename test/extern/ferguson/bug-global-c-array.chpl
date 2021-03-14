extern record myrec {
  var a: c_int;
  var b: c_ptr(c_int);
}

extern proc myfunc():myrec;

proc writeit(arg:c_ptr(c_int)) {
  writeln(arg[0]);
}

var x = myfunc();
writeln(x.a);
writeit(x.b);

var y = x;
writeln(y.a);
writeit(y.b);

var z: myrec;
z = x;
writeln(z.a);
writeit(z.b);
