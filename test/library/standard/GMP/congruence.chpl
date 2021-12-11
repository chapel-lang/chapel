use BigInteger;

var tensandthrees = new bigint("81000000000000000000000000000000000000000000");

writeln();
writeln(tensandthrees, " _p:");

for m in 1 .. 10 {
  var mb = new bigint(m);

  for c in 0..9 {
    var cb = new bigint(c);

    write(TF(tensandthrees.isCongruent(cb, mb)));
  }

  writeln();
}

writeln(tensandthrees, " ui_p:");

for m in 1 .. 10 : uint {
  for c in 0 .. 9 : uint {
    write(TF(tensandthrees.isCongruent(c, m)));
  }

  writeln();
}

writeln(tensandthrees, "_2exp_p:");

for b in 1 .. 10 : uint {
  for c in 0 .. 9 {
    var cb = new bigint(c);

    write(TF(tensandthrees.isCongruentByPowerOf2(cb, b)));
  }

  writeln();
}


proc TF(b) {
  if b then
    return " T";
  else
    return " F";
}

