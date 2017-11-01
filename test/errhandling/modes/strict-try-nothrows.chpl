pragma "error mode strict"
module mymodule {
  use ExampleErrors;

  proc propError() {
    throwAnError();
  }

  writeln("should not compile");

  try {
    propError();
  } catch {
    writeln("in catch");
  }
}
