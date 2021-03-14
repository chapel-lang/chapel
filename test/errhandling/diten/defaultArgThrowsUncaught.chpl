module M {
  config var cond = false;
  
  proc myproc(x = thrower()) {
    return x;
  }

  proc thrower() throws {
    if cond then
      throw new owned Error();
    return 1;
  }

  var a = myproc();
  writeln(a);
}
