@unstable("This function is unstable")
proc TestUnstableProc(x: bool) {
  if (x == true) {
    writeln("What's up!");
  }
  else {
    writeln("Hey!");
  }
}


proc main() {
    var a: bool = true;
    TestUnstableProc(a); // Should trigger the warning
    var b: bool = false;
    TestUnstableProc(b); // Should trigger the warning
}