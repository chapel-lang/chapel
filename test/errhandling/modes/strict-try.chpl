pragma "error mode strict"
module mymodule {
  use ExampleErrors;

  proc propError() throws {
    throwAnError();
  }

  try {
    propError();
  } catch {
    writeln("did not fail strict mode");
  }
}
