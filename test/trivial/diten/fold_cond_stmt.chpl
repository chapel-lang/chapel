config param a = true;
config param b = false;
config param c: uint = 5;

proc main() {
  param x = if a && b then 3 else c;
  writeln(foo(x));
}

proc foo(param n: uint) {
  var j = 0: n.type;
  for param i in 1..n {
    j += i;
  }
  return j;
}
