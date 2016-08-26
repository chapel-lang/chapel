enum Color {
  orange = 7, 
  indigo,
  yellow = 4,
  purple
}

enum Season {
  Autumn,
  Winter,
  Spring,
  Summer = 10
}

proc main(){
  for i in Color {
    var x : int;
    x = i;
    writeln(i, ": ", x);
  }
  writeln();
  for i in Season{
   var x : int;
    x = i;
    writeln(i, ": ", x);
  }
  writeln();
  for (i, j) in zip(Season, Color) do writeln(i, ", ", j);
}
