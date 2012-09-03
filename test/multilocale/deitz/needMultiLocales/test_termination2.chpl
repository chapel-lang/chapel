use Time;

proc main {
  var s1, s2: sync bool;
  on rootLocale.getLocales()(1) {
    begin {
      s1;
      on rootLocale.getLocales()(0) {
        begin {
          s2;
          writeln("executing on locale ", here.id);
        }
      }
      writeln("executing on locale ", here.id);
      s2 = true;
    }
  }
  writeln("executing on locale ", here.id);
  s1 = true;
}
