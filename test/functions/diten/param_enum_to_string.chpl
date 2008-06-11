enum color { red, green, blue };

def foo(param s: string) {
  writeln("param ", s);
}
def foo(s: string) {
  writeln("non-param ", s);
}

param color_param: color = color.green;
var   color_var  : color = color.green;

foo(color_param);
foo(color_var);
