class Base {
  const total = 60;
  proc printTotal() {
    writeln("Total is: ", total);
  }
}

class Extension : Base {
  const total = 1;
  proc printTotal() {
    writeln("Total is: ", total);
  }
}

proc main() {
  var ext       = new Extension();
  var base:Base = ext;

  writeln(ext.total);  // expect the total field from Extension
  ext.printTotal();    // expect the method from Extension

  writeln(base.total); // expect the total field from Base
  base.printTotal();   // expect the method from Extension

  delete ext;
}
