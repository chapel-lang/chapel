class C {
  var x : int;
  var y : float;
}

var c : C = C(1, 2.3);

var s : seq of C = (/ c /);

writeln(s);
