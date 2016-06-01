use GMP;

proc TF(b) {
  if b then return " T";
  return " F";
}

var tensandthrees = new BigInt("81000000000000000000000000000000000000000000");

writeln();
writeln(tensandthrees, " _p:");
for m in 1..10 {
  var mb = new BigInt(m);
  for c in 0..9 {
    var cb = new BigInt(c);
    write(TF(tensandthrees.congruent_p(cb, mb)));
    delete cb;
  }
  delete mb;
  writeln();
}

writeln(tensandthrees, " ui_p:");
for m in 1..10:uint {
  for c in 0..9:uint {
    write(TF(tensandthrees.congruent_ui_p(c, m)));
  }
  writeln();
}

writeln(tensandthrees, "_2exp_p:");
for b in 1..10:uint {
  for c in 0..9 {
    var cb = new BigInt(c);
    write(TF(tensandthrees.congruent_2exp_p(cb, b)));
    delete cb;
  }
  writeln();
}

delete tensandthrees;
