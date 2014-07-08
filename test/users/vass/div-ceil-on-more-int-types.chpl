proc launch(a,b) {
  writeln("divceil(", a, ":", typeToString(a.type), ",",
          b, ":", typeToString(b.type), ") = ", divceil(a,b));
  writeln("divfloor(", a, ":", typeToString(a.type), ",",
          b, ":", typeToString(b.type), ") = ", divfloor(a,b));
}

launch(7:int(32),  5:uint(64));
launch(7:uint(32), 5:uint(64));
launch(7:uint(64), 5:int(32));
launch(7:uint(64), 5:uint(32));
