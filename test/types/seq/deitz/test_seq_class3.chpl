class C {
  var x: int;
}

var s: seq of C;

writeln(s);

s._append_in_place(C(2));

writeln(s);
