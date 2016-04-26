module ShadowingCheck {
  // Verifies that a variable or function at an outer scope to the use of an
  // enum gets properly shadowed

  enum beatles {john, paul, george, ringo};

  var paul = "simon";
  proc george : string {
    return "clooney";
  }

  proc main() {
    use beatles;

    var testing = paul;
    var testingFunc = george;
    writeln(testing);
    writeln(testingFunc);
  }
}
