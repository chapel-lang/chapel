
class Inner {
  param x : int;
  param y : bool;

  var z : if y then x*int else x*real;
}

class C {
  forwarding var i = new unmanaged Inner(2, true);
}

var c = new unmanaged C();
writeln(c.x);
delete c.i;
delete c;
