class TestUnstableMethod {
  var v1 = 3;

  @unstable("this method is unstable, use stablemethod")
  proc unstablemethod() {
    stablemethod();
  }

  proc stablemethod() {
    writeln("In TestUnstableMethod.stablemethod()");
  }
}

proc main() {
  var f = new TestUnstableMethod(5);
  f.unstablemethod();
}
