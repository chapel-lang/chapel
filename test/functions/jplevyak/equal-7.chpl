class foo { var a : int;  }

var x : foo = new unmanaged foo();
var y : foo = new unmanaged foo();
var z : foo = x;

y.a = 1;
x   = y;

writeln(x.a);
writeln(z.a);

delete z;
delete y;

