proc bool.mymethod() {
  writeln("In bool.mymethod");
}

proc f(args...?n) {
  for param i in 1..n {
    args(i).mymethod();
  }
}

proc main {
  f(true);
}
