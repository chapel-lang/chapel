use Math;

config const a = 2, b = 12;
// This could call any non-inlined function in a standard
// or internal module that doesn't allocate/free memory/halt.
// We're just going to check it doesn't unnecessarily pass
// file and line # information.
var x = gcd(a, b);
writeln(x);

pragma "lineno ok"
pragma "insert line file info"
proc mygcd() {
  var c:int;
  var d = a + b + c;
  return d;
}

var y = mygcd();


writeln(y);
