class C {
  type t;
  var x: t;
}

var c = (new C(t=C(int).type,x=new C(int)));
writeln(c);
delete c.x;
delete c;
