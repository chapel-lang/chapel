proc f1(param shouldFail = false) {
  if shouldFail {
    fail();
  } else {
    writeln("Pass");
  }
}

if chpl__tryToken {
  writeln("True branch");
  f1(true);
} else {
  writeln("Else branch");
  f1(true);
}
