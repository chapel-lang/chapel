module X {
  proc giveFlag(i: int) throws {
    if i == 1 then return true;
    throw new owned IllegalArgumentError();
    return false;
  }

  try {
    var flag = true;
    if (flag || giveFlag(1)) {
      writeln("everything is ok");
    }
  } catch {
    writeln("nothing is ok");
  }
}
