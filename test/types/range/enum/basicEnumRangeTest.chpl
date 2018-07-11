use Color;

proc test(r) {
  writeln();
  writeln(r);
  writeln("---");
  for c in r {
    writeln(c);
    if c == color.red || c == color.violet then
      break;
  }
  writeln("---");
}

proc main() {
  test(color.orange..color.blue);
  for c in color.orange..color.blue do
    writeln(c);

  test(color.orange..);
  for c in (color.orange..) {
    writeln(c);
    if c == color.violet then
      break;
  }

  test(..color.blue by -1);
  for c in ..color.blue by -1 {
    writeln(c);
    if c == color.red then
      break;
  }

  test(color.orange..#2);
  for c in color.orange..#2 do
    writeln(c);

  test(color.orange..color.blue by 2);
  for c in color.orange..color.blue by 2 do
    writeln(c);

  test(color.orange..color.blue by -2);
  for c in color.orange..color.blue by -2 do
    writeln(c);
}
