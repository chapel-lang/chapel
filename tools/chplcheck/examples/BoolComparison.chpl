/*
  Booleans do not need to be compared to true or false explicitly.

  The following code is overly verbose and can be simplified.
*/
config const flag = true;
var isTrue = flag == true;
if flag != false then
  writeln("flag is true");
