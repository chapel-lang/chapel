module ShadowingCheck {
  // Verifies that renaming works

  enum beatles {john, paul, george, ringo};

  var paul = "simon";

  proc main() {
    use beatles only paul as frank;

    var testing: beatles = frank;
    var alsoTesting = frank;
    writeln(testing);
    writeln(alsoTesting);
  }
}
