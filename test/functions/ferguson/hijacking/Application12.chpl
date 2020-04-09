  module LibraryZ5 {
    class Base {
      proc setup() {
        writeln("starting LibraryZ.Base.setup!");

        helpSetup(1); // expected to call derived class

        writeln("completed LibraryZ.Base.setup!");
      }
      proc helpSetup(x:real) {
        writeln("LibraryZ.Base Default real setup helper");
      }
      proc helpSetup(x:int) {
        writeln("LibraryZ.Base Default int setup helper");
      }
    }
  }
  module Application12 {
    use LibraryZ5;

    class Widget : Base {
      proc helpSetup(x:real) {
        writeln("In Application.Widget.helpSetup");
      }
    }

    proc main() {
      var instance = new unmanaged Widget();
      instance.setup(); // calls Base.setup() and that runs Widget.helpSetup
      delete instance;
    }
  }
