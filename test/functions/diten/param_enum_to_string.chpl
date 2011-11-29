enum color { red, green, blue };

proc foo(param s: string) {
  writeln("param ", s);
}
proc foo(s: string) {
  writeln("non-param ", s);
}

param color_param: color = color.green;
var   color_var  : color = color.green;

foo(color_param);
foo(color_var);
