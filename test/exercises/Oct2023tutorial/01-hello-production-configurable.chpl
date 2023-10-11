module Hello {
  config const audience = "world";

  proc main() {
    writeln("Hello, ", audience, "!");
  }
}
