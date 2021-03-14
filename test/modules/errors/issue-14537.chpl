module Main {
  module bar {
    writeln("module bar");
  }
  
  module fabs {
    private proc bar() { writeln("proc bar"); }
  }

  module fibs {
    use Main;
    bar();
    proc main() { writeln("main"); }
  }
}
