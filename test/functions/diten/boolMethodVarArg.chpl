def bool.mymethod() {
  writeln("In bool.mymethod");
}

def f(args...?n) {
  for param i in 1..n {
    args(i).mymethod();
  }
}

def main {
  f(true);
}
