proc main {
  var bits: int(64) = 34;
  var num: int(64) = 1 << bits;
  var base: int(64) = 1;

  base = base << bits;

  writeln(num);
  writeln(1 << bits);
  writeln(1:uint(64) << bits);
  writeln(base);
}
