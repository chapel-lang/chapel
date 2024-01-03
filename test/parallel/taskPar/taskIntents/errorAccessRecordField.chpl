record R {
  var A: [1..1_000_000] int;
  var i = 0;
  proc ref modify(x: int) do A[x] = x;
  proc ref this(i: int) ref do return A[i];
  operator +=(ref r: R, x: int) do r.i += x;
}

var r: R;

// need to toggle between the cases, as the error messages won't all appear at once
config param test = 1;

forall i in 1..1_000_000 {
  if test == 1 {
    r += i; // should error
    r.i += i; // should error
    r.A[i] += i; // should error
  }
  else {
    r.modify(i); // should error
    r[i] += i; // should error
  }
}

