proc f() {
  try {
    writeln("inside try");
    throw new Error();
  } catch {
    stdout.writeln("reached stdout.writeln() successfully");
  }
}

f();
