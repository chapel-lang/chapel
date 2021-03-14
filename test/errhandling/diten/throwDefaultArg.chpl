module M {
  config const cond = false;

  proc thrower() throws {
    if cond then
      throw new owned Error();
    return 1;
  }

  proc myproc(arg = thrower()) throws {
    writeln(arg);
    return arg;
  }

  var a = try! myproc();
  writeln(a);
}
