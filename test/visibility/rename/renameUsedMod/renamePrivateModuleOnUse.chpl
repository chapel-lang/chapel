module Outer {
  private module Inner {
    var x: int;
  }

  public use Inner as In;
}

module User {
  use Outer;

  proc main() {
    // Publicly using and renaming a private module shouldn't break the module's
    // privacy and allow its renamed version to be used
    writeln(In.x);
  }
}
