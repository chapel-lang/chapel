/*
  Conditional statements in Chapel do not require parentheses around the
  condition. The following demonstrate this, the two if statements are
  equivalent.
*/
config const value = 5;
if (value > 0) then
  writeln("Value is positive");
if value > 0 then
  writeln("Value is positive");
