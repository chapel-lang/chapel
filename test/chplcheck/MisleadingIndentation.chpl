module Indentation {
  for i in 1..10 do
    writeln(i);
    writeln("second thing");

  @chplcheck.ignore("MisleadingIndentation")
  for i in 1..10 do
    writeln(i);
    writeln("second thing");
}
