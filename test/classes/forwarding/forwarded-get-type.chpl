class C {
  type t;
}

record R {
  var instance = new unmanaged C(int);
  forwarding instance;
}

var r : R;
writeln(r.t : string);
delete r.instance;
