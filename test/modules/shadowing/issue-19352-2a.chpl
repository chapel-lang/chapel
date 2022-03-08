module Library {
  record rec {
    proc method() { writeln("Library's rec.method()"); }
  }
}

// Case 2a
module Program {
  import Library;
  import Library.rec; // Should this bring in the methods as if defined here?

  proc rec.method() { writeln("Program's rec.method()"); }

  proc main() {
    var r = new Library.rec();
    r.method(); // currently outputs: Program's rec.method()
  }
}
