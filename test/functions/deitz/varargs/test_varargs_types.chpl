proc writeDefaultValues(args:?t ...?n) where n*t(1) == t {
  writeln(typeToString(args.type));
  for param i in 1..n {
    var x: args(i).type;
    writeln(x);
  }
}

proc writeDefaultValues(args:?t ...?n) where n*t(1) != t {
  writeln("arguments have different types");
}

writeDefaultValues(1, 2.0, 3.0i, 4.0+5.0i);
writeDefaultValues(1.0, 2.0, 3.0, 4.0);
