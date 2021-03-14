proc main(){
  enum numbers {one, two, pi};
  type typetup = (int, int, real);
  for (i, j) in zip(typetup, numbers)
  {
    if isIntType(i) then writeln(j, ". int");
    else writeln(j, ". not int");
  }
}
