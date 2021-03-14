  module LibraryX2 {
    var global:int;
    proc setup(x) {
      writeln("in LibraryX.setup()");
      global = try! x:int;
    }
    proc run(x) {
      setup(x);
      writeln("Global is ", global);
    }
  }
  module Application3 {
    use LibraryX2;

    proc main() {
      run("1");
    }
  }
