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
  module Application1 {
    use LibraryX1;

    proc main() {
      run(1);
    }
  }
