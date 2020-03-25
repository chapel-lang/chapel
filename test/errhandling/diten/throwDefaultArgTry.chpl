pragma "error mode strict"
module M {
  config const cond = false;

  proc thrower() throws {
    if cond then
      throw new owned Error();
    return 1;
  }

  proc myproc(arg = try thrower()) throws {
    writeln(arg);
    return arg;
  }
  proc foo() throws {
    var a = try myproc();
    writeln(a);
  }

  try! foo();
}
