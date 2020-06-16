proc output(xs ...?n) {
  for param i in 0..n-1 do
    foutput(xs(i));
}

proc foutput(x : int) {
  writeln("output int ", x);
}

proc foutput(x : real) {
  writeln("output real ", x);
}

proc foutput(x : string) {
  writeln("output string ", x);
}

proc foutput(x : bool) {
  writeln("output bool ", x);
}

output(1, 2.0, "three");
output(true);
output(true, 2, 3.0, "four");
