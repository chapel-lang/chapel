// This test is intended to show that modules that are alive are not marked as
// dead. 

// TODO are there any tests that should be added for extern and/or exported
// vars/procs 

// The writeln actually gets pulled into the initFn of the module. Make sure
// that even if a module just has an initFn that the initFn is not empty before
// marking the module for elimination
module aliveModWithStmt {
  writeln("I'm Alive!");
}

// Ensure that modules with variables that are not dead are not marked as dead
module aliveModWithVar {
  var usedVar = 3;
}

module mainMod {
  use aliveModWithVar;
  use aliveModWithStmt;
  proc main() {
    if (usedVar >= 0) {
      writeln("I'm Alive Too!");  
    }
  }
}

