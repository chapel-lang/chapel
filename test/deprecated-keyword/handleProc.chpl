deprecated "This function is deprecated, use blah(x: uint) instead"
proc blah(x: int) {
  if (x >= 0) {
    blah(x: uint);
  } else {
    writeln("Uh oh, we don't support negatives");
  }
}

proc blah(x: uint) {
  writeln(x);
}

proc main() {
  var a = 3;
  var b: uint = 5;
  blah(a); // Should trigger the warning
  blah(b); // Should not trigger the warning
}
