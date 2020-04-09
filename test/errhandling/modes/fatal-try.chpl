pragma "error mode fatal"
module mymodule {
  use ExampleErrors;

  proc propError() throws {
    throwAnError();
  }

  try {
    propError();
  } catch {
    writeln("OK");
  }
}
