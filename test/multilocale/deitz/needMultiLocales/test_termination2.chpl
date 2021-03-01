use Time;

proc main {
  var s1, s2: sync bool;
  sync {
    on Locales(1) {
      begin {
        s1.readFE();
        on Locales(0) {
          begin {
            s2.readFE();
            writeln("executing on locale ", here.id);
          }
        }
        writeln("executing on locale ", here.id);
        s2.writeEF(true);
      }
    }
    writeln("executing on locale ", here.id);
    s1.writeEF(true);
  }
}
