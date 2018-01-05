  module LibraryX1 {
    var global:int;
    proc setup() {
      writeln("in LibraryX.setup()");
      global = 1;
    }
    proc run(x) {
      setup();
      writeln("Global is ", global);
    }
  }
  module Application2 {
    use LibraryX1;

    proc setup() {
      writeln("in Application2.setup");
    }
    proc main() {
      run(1);
    }
  }
