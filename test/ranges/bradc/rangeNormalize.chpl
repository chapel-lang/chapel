proc applyNormalize(in r) {
  writeln("Before normalization: ", r);
  r.normalize();
  writeln("After  normalization: ", r);
}

applyNormalize(1..10 by 2);
applyNormalize(1..10 by -2);
