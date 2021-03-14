module ShadowingCheck {
  // Verifies that only lists work

  enum beatles {john, paul, george, ringo};

  var paul = "simon";
  proc george : string {
    return "clooney";
  }

  proc main() {
    use beatles only paul;

    var testing = paul;
    var testingFunc = george;
    writeln(testing);
    writeln(testingFunc);
  }
}
