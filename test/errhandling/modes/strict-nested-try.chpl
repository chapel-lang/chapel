pragma "error mode strict"
module OuterModule {

  module InnerModule {
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

  use InnerModule;
}
