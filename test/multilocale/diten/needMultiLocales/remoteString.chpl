config const printIterations = false;
proc main() {
  var s: string = "a string";
  var b: int;
  writeln(s);
  cobegin with (ref b, ref s) {
    on Locales(1) {
      while (s != "done") { b += 1; atomicFence(); }
      s = "another string";
    }
    on Locales(0) {
      s = "done";
    }
  }
  if printIterations then
    writeln(b);
  writeln(s);
}
