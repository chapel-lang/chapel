module LineLength {
  // This is a comment with a line length over 80 characters
  var thisIsMyRidiculouslyLongVariableNameThatShouldTakeItOverThe80CharacterLineMax
  : int = 0;
  proc veryLongFunctionName()                                                  {
    for                                                                   1..10 {
      writeln("This is a very long line of text that exceeds 80 characters. It will trigger the lint warning.");
    }
  }
}
