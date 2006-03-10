function output(xs ...?n) {
  for param i in 1..n do
    foutput(xs(i));
}

function foutput(x : integer) {
  writeln("output integer ", x);
}

function foutput(x : float) {
  writeln("output float ", x);
}

function foutput(x : string) {
  writeln("output string ", x);
}

function foutput(x : bool) {
  writeln("output bool ", x);
}

output(1, 2.0, "three");
output(true);
output(true, 2, 3.0, "four");
