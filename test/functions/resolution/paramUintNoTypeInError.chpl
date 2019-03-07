proc takesInt(param x: int(8)) {
  writeln("x is ", x);
}

param ui = 512: uint;
writeln(ui.type:string);
takesInt(ui);
