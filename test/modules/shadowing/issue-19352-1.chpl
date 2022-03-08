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

// Case 1
module Program {
  import Library;
  proc main() {
    import LibraryPlus.rec; // does this make LibraryPlus.rec.method "closer" ?
    var r = new Library.rec();
    r.method(); // currently: ambiguity error
  }
}
