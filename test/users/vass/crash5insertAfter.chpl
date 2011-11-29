proc test1(x: uint(?xx), y: uint(?yy)) {
  return xx;
}
writeln(test1(1:uint(32), 2:uint(32)));

proc test2(x: uint(?xx), y: uint(?yy)) {
  return yy;
}
writeln(test2(1:uint(32), 2:uint(32)));
