module Library {
  record rec {
    proc method() { writeln("Library's rec.method()"); }
  }
}

// Case 2b
module Program {
  import Library; // Should this bring in the methods
                  // on types defined in Library as if defined here?

  proc (Library.rec).method() { writeln("Program's rec.method()"); }

  proc main() {
    var r = new Library.rec();
    r.method(); // currently outputs: Program's rec.method()
  }
}
