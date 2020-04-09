proc testit(i) ref {
  writeln("I'm in testit, but not returning anything");
}

proc testit(i) ref {
  writeln("I'm in testit, but not returning anything");
  return;
}


testit(2);
testit2(2);
