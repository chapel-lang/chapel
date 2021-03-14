proc takesNonParamInt(x: int) {
  writeln("non-param x is ", x);
}

proc takesInt(param x: int) {
  writeln("param x is ", x);
}

param ui = 42: uint;
writeln(ui.type:string);
takesNonParamInt(ui);
takesInt(ui);
