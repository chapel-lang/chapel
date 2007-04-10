class C {
  var x: int;
}

class D: C {
  var y: int;
}

var s: seq of C;

s._append_in_place(D(1, 2));
s._append_in_place(C(3));

writeln(s);
