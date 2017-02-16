proc main {
  var a: atomic int;
  coforall loc in Locales {
    on loc {
      a.add(1);
    }
  }
  writeln(a);
}
