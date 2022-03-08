module Library {
  record rec {
    proc method() { writeln("Library's rec.method()"); }
  }
}


module LibraryPlus {
  import Library;
  import Library.rec;

  proc rec.method() { writeln("LibraryPlus's rec.method()"); }
}

// Case 3
module Program {
  public import Library;
  public use LibraryPlus; // should LibraryPlus.rec.method now shadow Library.rec.method?

  proc main() {
    var r = new Library.rec();
    r.method(); // currently:
                //  without PR #19306, ambiguity error
                //  with PR #19306, outputs LibraryPlus's rec.method()
  }
}
