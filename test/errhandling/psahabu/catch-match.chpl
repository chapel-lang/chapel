use ExampleErrors;

try {
  throwAnError();
} catch e: OtherError {
  writeln("should not catch here");
} catch {
  writeln("caught Error");
}

try {
  throwOtherError();
} catch e: OtherError {
  writeln("caught OtherError");
} catch {
  writeln("should not catch here");
}
