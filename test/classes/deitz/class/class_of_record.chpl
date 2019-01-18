record R {
  var x : int;
}

class C {
  var r : R;
}

var x = new unmanaged C();

writeln(x);

delete x;

