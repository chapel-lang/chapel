class C {
  var x : integer;
  var y : float;
}

var c1 : C = C(1, 2.3), c2 : C = C(2, 3.4);

var s : seq of C = (/ c1, c2 /);

writeln(s);

s = s # (/ c1, c2 /);

writeln(s);
