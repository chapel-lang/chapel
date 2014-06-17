class C {
  var x : int;
}

class D : C {
  type t;
  var y : t;
}

var d = new D(t=real);

writeln(d);
