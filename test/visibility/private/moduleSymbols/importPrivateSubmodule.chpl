module ExternalLib1 {
  private module Details {}
}

// My library's implementation details.
module Details {
  proc do_my_impl() { writeln("whatev"); }
}

module SomeOtherFile {
  proc main() {
    {
      use ExternalLib1;
      import Details;       // Should find the public module
      Details.do_my_impl(); // bug: compile-error
    }
    {
      import ExternalLib1;
      import Details;       // Resolves fine
      Details.do_my_impl();
    }
  }
}
