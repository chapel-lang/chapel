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
  module Application4 {
    use LibraryX2;

    proc setup(x:string) {
      writeln("in Application4.setup");
    }
    proc main() {
      run("1");
    }
  }
