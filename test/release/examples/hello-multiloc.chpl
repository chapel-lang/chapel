coforall loc in Locales {
  on loc {
    writeln("Hello, world! (from locale ", here.id, " of ", numLocales, ")");
  }
}
