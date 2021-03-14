use ExampleErrors;

try {
  writeln("outer try");
  try {
    writeln("inner try");
    throw new owned Error();
  } catch {
    writeln("inner catch");
    throw new owned Error();
  }
} catch {
  writeln("outer catch");
}

