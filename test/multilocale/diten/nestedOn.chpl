use Time;
config const sleepTime:uint = 1;
config const printIterations = false;
def main() {
  var s: string = "a string";
  var b: int;
  writeln(s);
  on Locales(1) {
    var i = 0;
    cobegin {
      while (s != "done") { i += 1; } 
      on Locales(s.locale) {
        s = "done";
      }
    }
    s = "another string";
    b = i;
  }
  if printIterations then
    writeln(b);
  writeln(s);
}
