class C {
  param p: int = 4;
  var x: p*int;
}

var c = new unmanaged C(p=5);
writeln(c);

var c2 = new unmanaged C();
writeln(c2);

delete c;
delete c2;
