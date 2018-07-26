class C {
  type t;
}

record R {
  var instance = new C(int);
  forwarding instance;
}

var r : R;
writeln(r.t : string);
