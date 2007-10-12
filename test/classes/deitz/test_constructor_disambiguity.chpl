class C {
  type t;
  var x: t;
}

writeln(C(t=C(int).type,x=C(int)));
