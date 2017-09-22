pragma "error mode strict"
module mymodule {
  use ExampleErrors;
  proc test() {
    coforall i in 1..10 {
      throw new StringError(i:string);
    }
  }


  test();
}
