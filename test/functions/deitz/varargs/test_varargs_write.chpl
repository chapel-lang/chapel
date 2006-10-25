def output(xs ...?n) {
  for param i in 1..n do
    foutput(xs(i));
}

def foutput(x : int) {
  writeln("output int ", x);
}

def foutput(x : real) {
  writeln("output real ", x);
}

def foutput(x : string) {
  writeln("output string ", x);
}

def foutput(x : bool) {
  writeln("output bool ", x);
}

output(1, 2.0, "three");
output(true);
output(true, 2, 3.0, "four");
