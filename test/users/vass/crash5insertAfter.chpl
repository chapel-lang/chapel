proc missing_plus(x: uint(?xx), y: uint(?yy)) {
  param zz = if xx > yy then xx else yy;
  return x:uint(zz) + y:uint(zz);
}
writeln(missing_plus(1:uint(32), 2:uint(32)));
writeln(missing_plus(1:uint(32), 2:uint(64)));
writeln(missing_plus(1:uint(64), 2:uint(64)));
