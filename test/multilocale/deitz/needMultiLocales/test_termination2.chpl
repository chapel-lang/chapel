use Time;

proc main {
  var s1, s2: sync bool;
  sync {
    on Locales(1) {
      begin {
        s1;
        on Locales(0) {
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
}
