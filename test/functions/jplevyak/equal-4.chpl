class foo { var a : int;  }

var x : unmanaged foo = new unmanaged foo();

delete x;



x = new unmanaged foo();

writeln(x);

delete x;
