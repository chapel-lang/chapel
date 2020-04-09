class foo { var a : int;  }

var x : unmanaged foo = new unmanaged foo();
var y : unmanaged foo = new unmanaged foo();
var z : unmanaged foo = x;

y.a = 1;
x   = y;

writeln(x.a);
writeln(z.a);

delete z;
delete y;

