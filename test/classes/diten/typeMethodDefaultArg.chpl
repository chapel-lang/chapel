record R {
  proc type method(a: int = 1) {
    writeln("Called method");
  }
}

R.method();
