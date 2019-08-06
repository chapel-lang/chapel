module X {
  proc giveFlag(i: int) throws {
    if i == 1 then return true;
    throw new owned IllegalArgumentError();
    return false;
  }

  try {
    var v = if giveFlag(1) then 100 else 1;
    writeln("everything is ok: ", v);
  } catch {
    writeln("nothing is ok");
  }
}
