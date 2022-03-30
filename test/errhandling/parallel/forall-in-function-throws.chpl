try {
  loop();
} catch e {
  writeln("Caught error");
}

proc loop() throws {
  forall i in 0..0 do
    throw new Error();
}
