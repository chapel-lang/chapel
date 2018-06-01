class foo { var a : int;  }

var x : foo = new unmanaged foo();

delete x;



x = new unmanaged foo();

writeln(x);

delete x;
