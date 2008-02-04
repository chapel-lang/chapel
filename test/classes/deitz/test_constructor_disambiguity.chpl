class C {
  type t;
  var x: t;
}

writeln(new C(t=C(int).type,x=new C(int)));
