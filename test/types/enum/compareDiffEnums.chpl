enum color {red, green, blue};
enum day {sunday, monday, tuesday};

config var c1 = color.green;
config var c2 = day.monday;

if (c1 == c2) {
  writeln("That's not right!");
}

if (c1 != color.green) {
  writeln("That's not right either!");
}

if (c1 != c2) {
  writeln("It works!");
}
