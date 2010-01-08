module M1 {
  var x: int = 1;
  var y: int = -1;
  def printX() {
    writeln("M1's x is: ", x);
  }
  def printY() {
    writeln("M1's y is: ", y);
  }
}
 
module M2 {
  use M3;
  use M1;

  var x: int = 2;

  def printX() {
    writeln("M2's x is: ", x);
  }

  def main() {
    M1.x = 4;
    M1.printX();
    writeln(x);
    printX(); // This is not ambiguous
    printY(); // ERROR: This is ambiguous
  }
}

module M3 {
  var x: int = 3;
  var y: int = -3;
  def printY() {
    writeln("M3's y is: ", y);
  }
}
