use Map;
proc myF() {
  return 1;
}
var m: map(string, myF.type);
m.add("asd", myF);
writeln(m);
