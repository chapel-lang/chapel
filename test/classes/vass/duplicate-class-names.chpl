proc p {
  class C { var x=3; }
  return new C();
}
proc q {
  class C { var y=8; }
  return new C();
}
class C { var z=17; }
proc r {
  return new C();
}

var c1 = p;
var c2 = q;
var c3 = r;

writeln(c1);
writeln(c2);
writeln(c3);

delete c3;
delete c2;
delete c1;

