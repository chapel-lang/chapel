use Time;

proc main {
  var a: sync int = 0;
  coforall loc in rootLocale.getLocales() {
    on loc {
      sleep(2);
      a += 1;
    }
  }
  writeln(a);
}
