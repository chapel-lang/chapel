/* Documentation for Test */
module sampleModule {

  // use test dependency
  use _MasonTest1;

  proc testProc(message: string) {
    // call function from dependency module that
    // just prints whatever string it's given
    moduleFunction(message);
  }
  // this tests that we don't segfault or show an error when
  // module output writes a closing paren
  writeln("some output from the module with closing paren)");
}