config type dataType = string;
var s = "a":dataType*10;

forall i in s.bytes() {
  writeln(i);
}

forall i in s.items() {
  writeln(i);
}

forall i in s {
  writeln(i);
}
