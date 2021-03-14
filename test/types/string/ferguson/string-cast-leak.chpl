config const i = 1;
config const msg = "test " + i:string;

proc g(x, y) {
  return x;
}

proc f(x) {
  return g(x:string, false);
}

proc run() {
  var t = f(msg);
  writeln(t);
}


run();
