proc main() {
  var i, j: int;
  local {
    bar(j);
  }
  writeln("In main with j = ", j);
}

proc bar(inout j: int) {
  j += 1;
  writeln("In bar with j = ", j);
  if j < 5 then bar(j);
}
