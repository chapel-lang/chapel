module Foo {
  proc a() {
    writeln("look at me!");
  }

  private proc b() {
    writeln("You are impinging on my privacy, sir or madam");
  }
}

module M {
  public use Foo;

  proc main() {
    // Verify that using a module publicly in an explicit manner doesn't break
    // our implementation of private.
    b();
  }
}
