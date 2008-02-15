enum color {red, green, blue, silver};
config param settable_color = color.blue;
config param settable_int = 3;

def main() {
  param acolor = color.green;
  enumParams(acolor, settable_color, settable_int);
  enumParams(settable_color, acolor, settable_int);
  enumParams(i=5, c2=color.red, c1=settable_color);
}

def enumParams(param c1: color, param c2: color, i: int) {
  writeln(c1, " ", c2, " ", i);
}
