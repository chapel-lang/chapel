/*
  Code should have consistent indentation within blocks.
*/

// This statement is incorrectly indented - it's not inside the if.
var x = true;
if x then
  writeln("First");
  writeln("Second");
  
// Correct indentation shows the second statement is outside the if.
if x then
  writeln("First");
writeln("Second");