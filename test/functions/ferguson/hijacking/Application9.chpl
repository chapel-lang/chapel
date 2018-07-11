  module LibraryZ3 {
    class Base {
      proc setup() {
        writeln("starting LibraryZ.Base.setup!");
      }
    }
  }
  module Application9 {
    use LibraryZ3;

    class Widget : Base {
      proc run(x:int) {
        writeln("In Application.Widget.run");
      }
    }

    proc main() {
      var instance = new unmanaged Widget();
      var x = 1;
      instance.setup(); // calls Base.setup()
      instance.run(x); // calls Widget.run()
      delete instance;
    }
  }
