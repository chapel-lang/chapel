
module m1 {
  var ccc = 3;
  module m2 {
    var aaa = 1, bbb = 2;
    proc foo() {
      writeln(aaa, " ", bbb, " ", ccc);
    }
  }
  proc foo() {
    use this.m2;
    writeln(ccc);
  }
}
module m3 {
  proc main() {
    use super.m1;
    foo();
  }
}
