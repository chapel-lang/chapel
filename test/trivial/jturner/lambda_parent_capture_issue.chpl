def testme() {
  var i = 5;
  class parent {
    def this() { return 0; }
  }

  class child : parent {
    def this() { return i; }
  }
  var f : parent = new child();
  writeln(f());
}
testme();
