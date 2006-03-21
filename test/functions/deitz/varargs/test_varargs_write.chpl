fun output(xs ...?n) {
  for param i in 1..n do
    foutput(xs(i));
}

fun foutput(x : int) {
  writeln("output int ", x);
}

fun foutput(x : float) {
  writeln("output float ", x);
}

fun foutput(x : string) {
  writeln("output string ", x);
}

fun foutput(x : bool) {
  writeln("output bool ", x);
}

output(1, 2.0, "three");
output(true);
output(true, 2, 3.0, "four");
