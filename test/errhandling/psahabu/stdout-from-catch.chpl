use IO;

proc f() {
  try {
    writeln("inside try");
    throw new owned Error();
  } catch {
    stdout.writeln("reached stdout.writeln() successfully");
  }
}

f();
