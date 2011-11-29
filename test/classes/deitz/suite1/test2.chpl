class C {
  type t;
  var x : t;
}

var c : C(int) = new C(int, 12);
writeln(c);
delete c;
