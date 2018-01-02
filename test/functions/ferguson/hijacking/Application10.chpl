  module LibraryZ4 {
    class Base {
      proc setup() {
        writeln("starting LibraryZ.Base.setup!");

        run(1); // calls Base.run

        writeln("completed LibraryZ.Base.setup!");
      }
      proc run(x:int) {
        writeln("in LibraryZ.Base.run!");
      }
    }
  }
  module Application9 {
    use LibraryZ4;

    class Widget : Base {
      proc run(x:int) {
        writeln("In Application.Widget.run");
      }
    }

    proc main() {
      var instance = new Widget();
      var x = 1;
      instance.setup(); // calls Base.setup()
      instance.run(x); // calls Widget.run()
      delete instance;
    }
  }

