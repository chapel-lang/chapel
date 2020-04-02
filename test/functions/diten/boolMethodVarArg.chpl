proc bool.mymethod() {
  writeln("In bool.mymethod");
}

proc f(args...?n) {
  for param i in 0..n-1 {
    args(i).mymethod();
  }
}

proc main {
  f(true);
}
