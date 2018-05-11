  module LibraryZ1 {
    class Base {
    }
  }
  module Application7 {
    use LibraryZ1;

    class Widget : Base {
      proc run(x:real) {
        writeln("In Application.Widget.foo");
      }
    }

    proc main() {
      var instance = new unmanaged Widget();
      var x = 1;
      instance.run(x);
      delete instance;
    }
  }
