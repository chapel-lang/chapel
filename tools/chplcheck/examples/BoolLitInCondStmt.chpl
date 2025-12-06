/*
  Boolean literals like 'true' or 'false' should not be used in conditional statements.
*/

// This uses a boolean literal in the condition which violates the rule.
if true {
  writeln("Ready to execute");
}

// This uses a variable in the condition which is correct.
var isReady = true;
if isReady {
  writeln("Ready to execute");
}
