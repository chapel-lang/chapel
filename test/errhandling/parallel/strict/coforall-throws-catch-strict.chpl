pragma "error mode strict"
module mymodule {
  use ExampleErrors;
  proc test() throws {
    try {
      coforall i in 1..10 {
        throw new owned StringError(i:string);
      }
    }
  }


  try {
    writeln("before test");
    test();
    writeln("after test");
  } catch g:TaskErrors {
    writeln("Caught errors ", g);
  } catch q {
    writeln("Caught something else ", q);
  }
}
