proc launch(a,b) {
  writeln("ceil(", a, ":", typeToString(a.type), ",",
          b, ":", typeToString(b.type), ") = ", ceil(a,b));
}

launch(7:int(32), 5:uint(64));
launch(7:uint(32), 5:uint(64));
launch(7:uint(64), 5:int(32));
launch(7:uint(64), 5:uint(32));
