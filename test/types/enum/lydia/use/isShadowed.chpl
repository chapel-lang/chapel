module ShadowingCheck {
  // Verifies that a variable or function at an outer scope to the use of an
  // enum gets properly shadowed

  enum beatles {john, paul, george, ringo};

  use beatles;

  var paul = "simon";
  proc george : string {
    return "clooney";
  }

  proc main() {
    var testing = paul;
    var testingFunc = george;
    var testingOther1 = john;
    var testingOther2 = ringo;
    writeln(testing);
    writeln(testingFunc);
    writeln(testingOther1);
    writeln(testingOther2);
  }
}
