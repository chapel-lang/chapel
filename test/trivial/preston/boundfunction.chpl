// playing with bound funs

proc int.fact() {
  var i = 1;
  var n = this;
  while n > 1 {
    i *= n;
    n -= 1;
  }
  return i;
}

var j = 5;
writeln(j.fact());

// but writeln(5.fact) provokes a syntax error at the "."
