try {
  loop();
} catch e {
  writeln("Caught error");
}

proc loop() throws {
  coforall loc in Locales do on loc {
    throw new Error();
  }
}
