// This test is intended to show that nested modules are not eliminated. This
// is because of the order in which modules are initialized. Inner modules call
// the initializers for the outer modules followed by the initializers that the
// outer module uses. 

module aliveModOuter {
  writeln("I'm alive outer!");
  module deadModInner {
  }
}

module deadModOuter {
  module aliveModInner {
    writeln("I'm alive inner!");
  }
}

module mainMod {
  use aliveModOuter.deadModInner; 
  use deadModOuter.aliveModInner;
  proc main() {
  }
}
