pragma "error mode strict"
module mymodule {
  proc test() throws {
    try {
      coforall i in 1..10 {
        throw new Error(i:string);
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
