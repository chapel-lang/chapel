
class Inner {
  param x : int;
  param y : bool;

  var z : if y then x*int else x*real;
}

class C {
  forwarding var i = new Inner(2, true);
}

var c = new C();
writeln(c.x);
delete c.i;
delete c;
