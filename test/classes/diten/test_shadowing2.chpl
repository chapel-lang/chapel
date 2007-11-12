class Base {
  const total = 60;
  def printTotal() {
    writeln("Total is: ", total);
  }
}

class Extension : Base {
  const total = 1;
  def printTotal() {
    writeln("Total is: ", total);
  }
}

def main() {
  var ext = Extension();
  var base = ext: Base;
  writeln(ext.total);  // expect the total field from Extension
  ext.printTotal();    // expect the method from Extension
  writeln(base.total); // expect the total field from Base
  base.printTotal();   // expect the method from Extension
}
