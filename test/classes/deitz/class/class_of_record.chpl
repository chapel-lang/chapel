record R {
  var x : int;
}

class C {
  var r : R;
}

var x = new C();

writeln(x);

delete x;

