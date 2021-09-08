
module M {
  proc main(args:[]string) {
    writeln("Test for when main has args but argument parser not used");
    // skip the first argument because the executable name/location may change
    // depending on the test configuration
    for arg in args[1..] do writeln(arg);
    writeln("Processed all arguments");
  }
}