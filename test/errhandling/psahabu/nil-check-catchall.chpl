use ExampleErrors;

try {
  try {
    throwAnError();
  } catch e {
    writeln("caught Error from try");
    throwAnError();
  }
} catch {
  writeln("caught error from named catchall");
}
