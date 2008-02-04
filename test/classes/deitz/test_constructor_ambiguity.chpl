class C {
  type t;
  var x: t;
}

writeln(new C(t=C(int),x=new C(int)));
