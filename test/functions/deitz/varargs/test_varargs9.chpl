proc output_help(param i : int, x) {
  if i > 0 then
    output_help(i-1, x);
  output(x(i));
}

proc output(x, xs ...?n) {
  output(x);
  output_help(n-1, xs);
}

proc output(x : int) {
  writeln("output int ", x);
}

proc output(x : real) {
  writeln("output real ", x);
}

proc output(x : string) {
  writeln("output string ", x);
}

output(1, 2.0, "three");
output(2.0, "three", "four", 5);
