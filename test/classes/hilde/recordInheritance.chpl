record R {
  var x : int = 1;
  proc xsq return x*x;
}

record S : R {
  var y : int = 2;

  // Return the integer square root of y using Newton's method.
  proc sqrty {
    if y < 0 then halt("Cannot represent imaginary results.");
    var q = y;  // quotient
    var d = 1;  // divisor
    while q - d > 1 {
      d = (q + d) / 2;  // Average to find new divisor.
      q = y / d;
      if d > q then d <=> q;
    }
    return d;
  }
}

var s : S;

writeln(s);
s.x = 3;
writeln(s.x);

// This doesn't work, because only fields and not methods are inherited
// in record inheritance.
writeln(s.xsq);

s.y = 27;
writeln(s.sqrty);
