use ExampleErrors;

try {
  writeln("outer try");
  try {
    writeln("inner try");
    throwAnError();
  } catch {
    writeln("inner catch");
    throwAnError();
  }
} catch {
  writeln("outer catch");
}

