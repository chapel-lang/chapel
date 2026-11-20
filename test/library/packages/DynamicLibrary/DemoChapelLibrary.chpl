export proc test1() {
  coforall loc in Locales do on loc {
    const id = here.id;
    writeln("Hello from locale ", id);
  }
}
