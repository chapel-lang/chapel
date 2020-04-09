module ShadowingCheck {
  // Verifies that except lists work

  enum beatles {john, paul, george, ringo};

  var paul = "simon";
  proc george : string {
    return "clooney";
  }

  proc main() {
    use beatles except george;

    var testing = paul;
    var testingFunc = george;
    writeln(testing);
    writeln(testingFunc);
  }
}
