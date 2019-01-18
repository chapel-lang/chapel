module StackMod {
  record Stack {
    var x: int;

    proc init(x: int) {
      this.x = x;
    }
  }
}

module M1 {
  use StackMod;

  var stack = new StackMod.Stack(x=2);
  
  proc main() {
    writeln(stack);
  }
}