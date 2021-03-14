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
    var x = try! throwit(1);
    
    x = try! throwit(2) + 1;

    x = try! 1 + throwit(3);

    x = try! throwit(4) + throwit(5);

    x = add( try! throwit(6), try! throwit(7));

    x = try! add( throwit(6), throwit(7));

    x = try! tryReturn(8);

    x = trybReturn(9);

    writeln(x);
  }

  proc main() {
    try! run();
  }
}
