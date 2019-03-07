class C {
  type t;
  var x: t;
}

var c = (new unmanaged C(
           t=(unmanaged C(int)),
           x=new unmanaged C(int)));
writeln(c);
delete c.x;
delete c;
