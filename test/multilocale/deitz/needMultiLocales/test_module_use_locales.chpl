module M1 {
  writeln(here.id, " initializing M1");
  var x: int = 17;
  writeln(here.id, " in M1 initialization x = ", x);
}

module M2 {
  writeln(here.id, " initializing M2");
  proc main() {
    writeln(here.id, " starting main");
    on Locales(1) {
      writeln(here.id, " using M1");
      use M1;
      writeln(here.id, " in main x = ", x);
    }
    writeln(here.id, " finishing main");
  }
}
