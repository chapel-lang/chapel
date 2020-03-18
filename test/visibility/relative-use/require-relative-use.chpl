module M {
  
  module SubM {
    var x: int;
  }
  
  use SubM;
  proc main() {
    writeln(x);
  }
}
