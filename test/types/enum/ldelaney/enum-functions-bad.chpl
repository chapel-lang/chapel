proc main(){
  enum digit {zero, one, two, three, four, five, six, seven, eight, nine};
  //try to access a member symbol or method that does not exist
  writeln("Bad Member Symbol: ", digit.ten);
}
