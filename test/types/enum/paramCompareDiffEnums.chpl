enum color {red, green, blue};
enum day {sunday, monday, tuesday};

if (color.green == day.monday) {
  compilerError("That's not right!");
}

param c = color.green;

if (c != day.monday) {
  compilerError("That's not right either!");
}

compilerError("It works!");
