// This test is intended to ensure that deadMod is eliminated by dead module
// elimination. It is also intended to ensure that aliveMod gets added to
// mainMod's list of used modules. In other words, the net effect should be
// that deadMod does not exist, and mainMain uses aliveMod. Lastly,
// deadModWithDeadProc should also be eliminated. 

module aliveMod {
  proc alive() {
    writeln("I'm Alive!");
  }
}

module deadMod {
  public use aliveMod;
}

module deadModWithDeadProc {
  proc dead() {
   writeln("I'm Dead :("); 
  }
}

module mainMod {
  public use deadMod;
  proc main() {
    alive();
  }
}
