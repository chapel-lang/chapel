class C {
  type t;
  var x : t;
}

var c = new unmanaged C(int, 12);
writeln(c);
delete c;
