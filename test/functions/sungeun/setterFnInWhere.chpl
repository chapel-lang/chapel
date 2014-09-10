var i: int;
proc blah() ref where setter {
  return i;
}

proc blah() ref where !setter {
  return 3;
}

blah() = 4;

writeln(i);
writeln(blah());

