use ExampleErrors;

try {
  writeln("outer try");
  try {
    writeln("inner try");
    throw new Error();
  } catch {
    writeln("inner catch");
    throw new Error();
  }
} catch {
  writeln("outer catch");
}

