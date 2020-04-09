iter yieldTenNones() {
  writeln("In yieldTenNones");
  for i in 1..10 do
    yield none;
}

iter yieldANone() {
  writeln("In yieldANone");
  yield none;
}

proc main {
  for v in yieldTenNones() do
    writeln("Got a none!");    // should print this message ten times

  for v in yieldANone() do
    writeln("Got a none!");    // should print this message one time
}
