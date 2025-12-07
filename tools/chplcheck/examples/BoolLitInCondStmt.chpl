/*
 Boolean literals like 'true' or 'false' should not be used in conditional statements.
*/

// The condition is always true, so the if statement is unnecessary.
if true {
  writeln("Always executes");
}

// Just execute the code directly.
writeln("Always executes");

// Similarly, this code will never execute, we can remove it.
if false {
  writeln("Never executes");
}
