module M1 {
  var x: int = 2;
}

use M1;

module M2 {
  def main {
    writeln(x);
  }
}
