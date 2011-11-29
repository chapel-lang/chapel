enum color { red, green, blue };

proc foo(param i: int) {
  writeln("param ", i);
}
proc foo(i: int) {
  writeln("non-param ", i);
}

param color_param: color = color.green;
var   color_var  : color = color.green;

foo(color_param);
foo(color_var);
