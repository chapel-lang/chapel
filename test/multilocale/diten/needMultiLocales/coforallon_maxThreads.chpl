use Time;

proc main {
  var a: sync int = 0;
  coforall loc in Locales {
    on loc {
      sleep(2);
      a += 1;
    }
  }
  writeln(a.readFF());
}
