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
  module LibraryY1 {
    proc go() { }
  }
  module Application5 {
    use LibraryX3;
    use LibraryY1;

    proc main() {
      var x:int = 1;
      setup(x); // from LibraryX
      run();
      go(); // from LibraryY
    }
  }
