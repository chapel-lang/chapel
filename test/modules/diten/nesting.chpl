
module m1 {
  var ccc = 3;
  module m2 {
    import m1.ccc;
    var aaa = 1, bbb = 2;
    proc foo() {
      writeln(aaa, " ", bbb, " ", ccc);
    }
  }
  proc foo() {
    use m2;
    writeln(ccc);
  }
}
module m3 {
  proc main() {
    use m1;
    foo();
  }
}
