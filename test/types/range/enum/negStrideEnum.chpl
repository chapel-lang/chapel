use Color;
use color;

proc main() {
  const r = red..violet;

  for c in r by -1 do
    writeln(c);
  writeln("---");

  for c in red..violet by -1 do
    writeln(c);
  writeln("---");

  for c in red..violet by -2 do
    writeln(c);
  writeln("---");

  for c in r by -2 do
    writeln(c);
  writeln("---");
}
