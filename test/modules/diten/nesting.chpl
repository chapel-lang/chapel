
module m1 {
  var ccc = 3;
  module m2 {
    var aaa = 1, bbb = 2;
    def foo() {
      writeln(aaa, " ", bbb, " ", ccc);
    }
  }
  def foo() {
    use m2;
    writeln(ccc);
  }
}
module m3 {
  def main() {
    use m1;
    foo();
  }
}
