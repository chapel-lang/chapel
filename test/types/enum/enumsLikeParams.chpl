enum color {red, green, blue};

proc foo(x: color) {
  writeln("In normal version");
}

proc foo(param x: color) {
  writeln("In param version");
}

foo(color.red);   // calls param version

var hue: color;
foo(hue);         // calls normal version

param shade: color = color.green;
foo(shade);       // calls param version
