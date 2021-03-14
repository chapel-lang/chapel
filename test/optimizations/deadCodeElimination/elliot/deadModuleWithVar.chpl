// This is intended to show that deadModules that have only dead variables are
// still eliminated. Currently this does not work because global variables are
// not eliminated. 

module deadMod {
  var unusedVar = 7;
}

module mainMod {
  use deadMod;
  proc main() { 
    writeln("I'm Alive!");
  }
}
