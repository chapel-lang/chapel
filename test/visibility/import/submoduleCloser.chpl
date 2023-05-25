module ExternalLib1 {
  module Details {}
}

// My library's implementation details.
module Details {
  proc do_my_impl() { writeln("whatev"); }
}

module SomeOtherFile {
  proc main() {
    {
      use ExternalLib1;
      import Details;       // Should this find the public module?
      Details.do_my_impl(); // currently: compile-error
    }
    {
      import ExternalLib1;
      import Details;       // Resolves fine
      Details.do_my_impl();
    }
  }
}
