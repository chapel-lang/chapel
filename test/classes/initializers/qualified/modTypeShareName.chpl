module Stack {
  record Stack {
    var x: int;

    proc init(x: int) {
      this.x = x;
    }
  }
}

module M1 {
  use Stack;

  var stack = new Stack.Stack(x=2);
  
  proc main() {
    writeln(stack);
  }
}