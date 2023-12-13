record R {
  var i = 0;
  operator +=(ref r: R, x: int) do r.i += x;
}

var r: R;

forall i in 1..1_000_000 {
  r.i += i;
}

