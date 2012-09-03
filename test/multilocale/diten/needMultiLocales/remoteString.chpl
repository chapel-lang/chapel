use Time;
config const sleepTime:uint = 1;
config const printIterations = false;
proc main() {
  var s: string = "a string";
  var b: int;
  writeln(s);
  cobegin {
    on rootLocale.getLocales()(1) {
      while (s != "done") { b += 1; }
      s = "another string";
    }
    on rootLocale.getLocales()(0) {
      sleep(sleepTime);
      s = "done";
    }
  }
  if printIterations then
    writeln(b);
  writeln(s);
}
