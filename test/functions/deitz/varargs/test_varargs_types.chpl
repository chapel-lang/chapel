proc writeDefaultValues(args:?t ...?n) {
  writeln(args.type:string);
  for param i in 0..n-1 {
    var x: args(i).type;
    writeln(x);
  }
}

proc writeDefaultValues(args...?n) {
  writeln("arguments have different types");
}

writeDefaultValues(1, 2.0, 3.0i, 4.0+5.0i);
writeDefaultValues(1.0, 2.0, 3.0, 4.0);
