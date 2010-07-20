def testme() {
  var i = 5;
  class parent {
    def this() { return 0; }
  }

  class fcfun : parent {
    def this() { return i; }
  }
  var f : parent = new fcfun();
  writeln(f());
}
testme();
