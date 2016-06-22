var i: int;
proc blah() ref {
  return i;
}

proc blah() {
  return 3;
}

blah() = 4;

writeln(i);
writeln(blah());

