function output_help(param i : int, x) {
  if i > 1 then
    output_help(i-1, x);
  output(x(i));
}

function output(x, xs ...?n) {
  output(x);
  output_help(n, xs);
}

function output(x : int) {
  writeln("output int ", x);
}

function output(x : float) {
  writeln("output float ", x);
}

function output(x : string) {
  writeln("output string ", x);
}

output(1, 2.0, "three");
output(2.0, "three", "four", 5);
