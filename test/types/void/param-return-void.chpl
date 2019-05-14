
proc helper(param cond : bool) {
  if cond {
    return (1, 2.0, "3");
  } else {
    return none;
  }
}

var T = helper(true);
writeln("T = '", T, "'");
writeln("T.type = ", T.type:string);

var F = helper(false);
writeln("F = '", F, "'");
writeln("F.type = ", F.type:string);

