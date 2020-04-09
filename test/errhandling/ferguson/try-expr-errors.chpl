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

  proc tryReturn(i:int):int {
    return try throwit(i);
  }

  proc main() {
    var x = 1;
    
    x = try add( throwit(1),
                 throwit(2));

    x = throwit(3) +
        (try! throwit(4));

    x = add( try! throwit(5),
             throwit(6));

    x = tryReturn(7);
  }
}
