use ExampleErrors;

try {
  throwAnError();
} catch {
  writeln("should never run");
} catch e: OtherError {
  writeln("should never run");
}
