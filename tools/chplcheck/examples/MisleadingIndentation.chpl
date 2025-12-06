/*
  Single-statement blocks should not have misleading indentation.
*/

// This has misleading indentation which violates the rule.
var x = true;
if x
  writeln("First");
  writeln("Second");

// This uses braces to clarify the block which is correct.
if x {
  writeln("First");
}
writeln("Second");
