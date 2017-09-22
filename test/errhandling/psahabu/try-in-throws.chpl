use ExampleErrors;

try {
  writeln("outer try");
  throwAnError();
} catch {
  writeln("outer catch");
  try {
    writeln("inner try");
    throwAnError();
  } catch {
    writeln("inner catch");
  }
}

