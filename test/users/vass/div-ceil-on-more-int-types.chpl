proc launch(a,b) {
  writeln("divceil(", a, ":", a.type:string, ",",
          b, ":", b.type:string, ") = ", divceil(a,b));
  writeln("divfloor(", a, ":", a.type:string, ",",
          b, ":", b.type:string, ") = ", divfloor(a,b));
}

launch(7:int(32),  5:uint(64));
launch(7:uint(32), 5:uint(64));
launch(7:uint(64), 5:int(32));
launch(7:uint(64), 5:uint(32));
