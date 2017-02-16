proc main(){
  enum game {rock, paper, scissors};
  enum medals {gold = 1, silver, bronze};
  for (i, j) in zip(game, medals) do writeln(i, ", ", j);
  for (i, j) in zip(medals, 1..medals.size) do writeln(i, ", ", j);
}
