use List;

class C {
  var x : int;
  var y : real;
}

var c : unmanaged C = new unmanaged C(1, 2.3);
var s : list(unmanaged C) = makeList(c);

writeln(s);

s.destroy();

delete c;
