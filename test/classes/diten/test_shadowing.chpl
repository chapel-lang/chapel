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
  var ext = new Extension();
  writeln(ext.total);  // expect the total field from Extension
  ext.printTotal();    // expect the method from Extension
}
