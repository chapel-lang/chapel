  module LibraryZ4 {
    class Base {
      proc setup() {
        writeln("starting LibraryZ.Base.setup!");

        helpSetup(1); // expected to call derived class

        writeln("completed LibraryZ.Base.setup!");
      }
      proc helpSetup(x:real) {
        writeln("LibraryZ.Base Default setup helper");
      }
    }
  }
  module Application11 {
    use LibraryZ4;

    class Widget : Base {
      override proc helpSetup(x:real) {
        writeln("In Application.Widget.helpSetup");
      }
    }

    proc main() {
      var instance = new borrowed Widget();
      instance.setup(); // calls Base.setup() and that runs Widget.helpSetup
    }
  }
