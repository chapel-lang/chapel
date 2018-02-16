module LibraryX1 {

  class MyClass {
    var x:int;
  }

  var global:MyClass;
  proc setup() {
    writeln("In LibraryX1.setup");
    global = new MyClass(10);
  }
  proc run() {
    setup();
    writeln("In LibraryX1.run global is ", global);
  }
}

module LibraryY1 {
  
  class MyClass {
    var x:int;
    var y:int;
  }


  var global:MyClass;
  proc setup() {
    writeln("In LibraryY1.setup");
    global = new MyClass(1,2);
  }
  proc run() {
    setup();
    writeln("In LibraryY1.run global is ", global);
  }
}

module Main {
  use LibraryX1;
  use LibraryY1;

  proc main() {
    LibraryX1.run();
    LibraryY1.run();
  }
}
