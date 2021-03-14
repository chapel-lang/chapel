proc main(){
  enum digit {zero, one, two, three, four, five, six, seven, eight, nine};
  enum game {rock, paper, scissors};

  //test enum functions
  writeln("Foo: ", digit.foo(), ", ", game.foo());
  writeln("Foo2: ", digit.foo2, ", ", game.foo2);
  //test the compiler generated enum.size function
  writeln("size: ", digit.size, ", ", game.size);
  //test the access of member symbols
  writeln("Member Symbols: ", digit.one, ", ", game.scissors);
}

//an enum chapel function with parentheses
proc type enum.foo(){
  return "foo";
}

//an enum chapel function without parentheses
proc type enum.foo2{
  return "foo2";
}
