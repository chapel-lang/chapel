module N {
  module MM { var x: int; }
}

module Main {
  
  proc main() {
    use N;
    use MM;
    writeln(x);
  }
}
