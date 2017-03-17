try {
  writeln("outer try");
  try {
    writeln("inner try");
    throw new Error();
  }
  writeln("fail: outer try did not throw");
} catch {
  writeln("handled nested try");
}
