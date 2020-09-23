class C {
  type t;
  var x : t;
}

var c : unmanaged C(int) = new unmanaged C(int, 12);
writeln(c);
delete c;
