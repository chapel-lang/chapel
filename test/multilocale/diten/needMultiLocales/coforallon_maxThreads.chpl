use Time;

proc main {
  var a: atomic int;
  coforall loc in Locales {
    on loc {
      sleep(2);
      a.add(1);
    }
  }
  writeln(a);
}
