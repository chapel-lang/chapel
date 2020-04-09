record R {
  const c: int;
  var v: int;
}

var myR = new R(c=1, v=2);

var A: [1..3] R = [i in 1..3] myR;

writeln(A);
