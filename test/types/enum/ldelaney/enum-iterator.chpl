enum Color {
  orange = 7, 
  indigo,
  yellow = 4,
  purple
}

enum Season {
  Autumn = 1,
  Winter,
  Spring,
  Summer = 10
}

proc main(){
  for i in Color {
    var x = i: int;
    writeln(i, ": ", x);
  }
  writeln();
  for i in Season{
    var x = i: int;
    writeln(i, ": ", x);
  }
  writeln();
  for (i, j) in zip(Season, Color) do writeln(i, ", ", j);
}
