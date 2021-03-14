  module LibraryX3 {
    var global:real;
    proc setup(x:real) {
      writeln("in LibraryX.setup()");
      global = x;
    }
    proc run() {
      writeln("Global is ", global);
    }
  }
  module LibraryY2 {
    proc setup(x:int) {
      writeln("in LibraryY.setup");
    }
    proc go() { }
  }
  module Application6 {
    use LibraryX3;
    use LibraryY2;

    proc main() {
      var x:int = 1;
      setup(x); // from LibraryX
      run(); // from LibraryX
      go(); // from LibraryY
    }
  }
