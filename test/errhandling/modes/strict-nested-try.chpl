pragma "error mode strict"
module OuterModule {

  module InnerModule {
    use ThrowError;

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
