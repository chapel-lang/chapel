enum color {red, green, blue};
enum day {sunday, monday, tuesday};

if (color.green == day.monday) {
  compilerError("That's not right!");
}

if (color.green != color.green) {
  compilerError("That's not right either!");
}

if (color.green != day.monday) {
  compilerError("It works!");
}
