config const printIterations = false;
proc main() {
  var s: string = "a string";
  var b: int;
  writeln(s);
  on Locales(1) {
    var i = 0;
    var r: sync int;
    cobegin with (ref i, ref s) {
      { r.readFE(); while (s != "done") { i += 1; } }
      on s.locale {
        s = "done"; r.writeEF(1);
      }
    }
    s = "another string";
    b = i;
  }
  if printIterations then
    writeln(b);
  writeln(s);
}
