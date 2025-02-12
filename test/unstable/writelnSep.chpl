
// specifying sep is unstable and only that

{
  write("H", "e", "l", "l", "o");
  writeln(" ", "w", "o", "r", "l", "d");

  // same behavior as above, but now its going to warn
  write("H", "e", "l", "l", "o", sep="");
  writeln(" ", "w", "o", "r", "l", "d", sep="");

  // different behavior and it will warn
  write("H", "e", "l", "l", "o", sep=" ");
  writeln(" ", "w", "o", "r", "l", "d", sep=" ");
}

{
  use IO;

  stdout.write("H", "e", "l", "l", "o");
  stdout.writeln(" ", "w", "o", "r", "l", "d");

  // same behavior as above, but now its going to warn
  stdout.write("H", "e", "l", "l", "o", sep="");
  stdout.writeln(" ", "w", "o", "r", "l", "d", sep="");

  // different behavior and it will warn
  stdout.write("H", "e", "l", "l", "o", sep=" ");
  stdout.writeln(" ", "w", "o", "r", "l", "d", sep=" ");
}
