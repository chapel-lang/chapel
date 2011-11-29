module M1 {
  use bar;

  proc main() {
    writeln("In  M1's main");
  }
}

module M2 {
  use bar;
 
  proc main() {
    writeln("In M2's main");
  }
}
