module LibraryX1 {
  proc setup() {
    writeln("In LibraryX1.setup");
  }
  proc run() {
    setup();
  }
}

module LibraryY1 {
  proc setup() {
    writeln("In LibraryY1.setup");
  }
  proc run() {
    setup();
  }
}

module Main {
  use LibraryX1;
  use LibraryY1;

  proc main() {
    LibraryX1.run();
    LibraryY1.run();
  }
}
