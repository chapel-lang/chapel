//cannot coerce string to bytes (nothing fundamentally wrong, but not supported)
param s = "some string";
proc bar(param b: bytes) {
  writeln(b, " as ", b.type:string);
}
bar(s);
