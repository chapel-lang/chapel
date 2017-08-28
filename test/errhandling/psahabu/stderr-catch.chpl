proc f() {
  try {
    writeln("inside try");
  } catch {
    stderr.writeln("stderr");
  }
}

f();
