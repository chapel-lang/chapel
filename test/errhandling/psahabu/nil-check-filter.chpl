use ExampleErrors;

try {
  try {
    throw new OtherError();
  } catch e: OtherError {
    writeln("caught OtherError from try");
    throwAnError();
  }
} catch {
  writeln("caught error from catch filter");
}
