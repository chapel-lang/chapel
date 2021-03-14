module mymodule {
  use ExampleErrors;

  proc propError() {
    throwAnError();
  }

  try {
    propError();
  } catch {
    writeln("in catch");
  }
}
