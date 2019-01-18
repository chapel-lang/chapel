module bradModule {
  config const n: int = 10;

  writeln("n is: ", n);

  proc help() {
    writeln("bradModule flags");
    writeln("================");
    writeln("  --n=<val> : set the problem size (default = 10)");
  }
}
