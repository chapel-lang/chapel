module LineLength {
  // This is a comment with a line length over 80 characters
  var thisIsMyRidiculouslyLongVariableNameThatShouldTakeItOverThe80CharacterLineMax
  : int = 0;
  proc veryLongFunctionName()                                                  {
    for                                                                   1..10 {
      writeln("This is a very long line of text that exceeds 80 characters. It will trigger the lint warning.");
    }
  }

  // This is a comment with a line length over 20 characters
  var thisIsMyVeryLongVariableName:
                       /*this inline comment also triggers it*/
  int = 0;

  /*I am a really long docstring but I should still get warned about by default because its long*/
  proc foo() {}
  /*a normal docstring*/
                                                                                /*why would someone write this?*/proc bar() {}
                                                                  /*weird edge*/
                                                                    /*just over*/
  // whitespace doesn't count
  var loremIpsumAndDolorSitAmetConsecteturAdipiscingElit: int = 123456789 + 222;           
}
