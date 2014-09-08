var i: int;
proc blah() var where setter {
  return i;
}

proc blah() var where !setter {
  return 3;
}

blah() = 4;

writeln(i);
writeln(blah());

