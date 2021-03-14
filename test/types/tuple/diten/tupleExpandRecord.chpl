record R {
  param size = 3;
  var a: int;
  proc this(i) ref return a;
}

var r: R;
r(1) = 1;
r(2) = 2;
r(3) = 3;

writeln((...r));
