record R {
  const c: int;
  var v: int;
}

var myR = new R(c=1, v=2);

writeln([myR, myR, myR]);
