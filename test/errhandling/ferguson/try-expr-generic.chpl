pragma "error mode strict"
module MyModule {
  use ExampleErrors;

  config const test = 1;

  proc throwit(i:int):int throws {
    if i == test then
      throw new owned StringError("Test");
    return i;
  }

  proc trybReturn(i) {
    return try! throwit(i);
  }


  proc run() throws {
    var x = trybReturn(1);
    writeln(x);
  }

  proc main() {
    try! run();
  }
}
