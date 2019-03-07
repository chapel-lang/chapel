/* Documentation for Test */
module sampleModule {

  // use test dependency
  use _MasonTest1;

  proc testProc(message: string) {
    // call function from dependency module that
    // just prints whatever string it's given
    moduleFunction(message);
  }
  
}