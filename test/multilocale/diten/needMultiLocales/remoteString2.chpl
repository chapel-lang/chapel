config const printIterations = false;
proc main() {
  var s: string = "a string";
  var b: int;
  writeln(s);
  on Locales(1) {
    while (s != "done") {
      b += 1;
      s = "done";
    }
    s = "another string";
  }
  if printIterations then
    writeln(b);
  writeln(s);
}
