pragma "error mode strict"
module MyModule {
  use ExampleErrors;

  config const test = 1;

  proc throwit(i:int):int throws {
    if i == test then
      throw new owned StringError("Test");
    return i;
  }

  proc add(i:int, j:int):int {
    return i + j;
  }

  proc tryReturn(i:int):int throws {
    return try throwit(i);
  }

  proc trybReturn(i:int):int {
    return try! throwit(i);
  }


  proc run() throws {
    var x = 0;

    x = try! throwit( try 1+throwit(1));

    x = try throwit( try! 1+throwit(3));

    x = try add( 1, add(1, throwit(5)));
    
    x = try! add( add(throwit(6), 1), 1);
  }

  proc main() {
    try! run();
  }
}
