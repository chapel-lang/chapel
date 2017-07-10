iter yieldTenVoids() {
  writeln("In yieldTenVoids");
  for i in 1..10 do
    yield _void;
}

iter yieldAVoid() {
  writeln("In yieldAVoid");
  yield _void;
}

proc main {
  for v in yieldTenVoids() do
    writeln("Got a void!");    // should print this message ten times

  for v in yieldAVoid() do
    writeln("Got a void!");    // should print this message one time
}
