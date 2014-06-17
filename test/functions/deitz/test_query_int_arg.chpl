proc foo(i: int(?k)) {
  writeln("foo of int on ", k, " bits called with ", i);
  var i2: int(k) = i;
  writeln(i2);
  i2 = max(i2.type);
  writeln(i2);
}

foo(1:int(8));
foo(2:int(16));
