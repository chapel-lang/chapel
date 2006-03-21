fun output_help(param i : int, x) {
  if i > 1 then
    output_help(i-1, x);
  output(x(i));
}

fun output(x, xs ...?n) {
  output(x);
  output_help(n, xs);
}

fun output(x : int) {
  writeln("output int ", x);
}

fun output(x : float) {
  writeln("output float ", x);
}

fun output(x : string) {
  writeln("output string ", x);
}

output(1, 2.0, "three");
output(2.0, "three", "four", 5);
