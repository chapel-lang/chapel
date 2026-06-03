/*
  Code should have consistent indentation within blocks.
*/

// This has inconsistent indentation which violates the rule.
proc foo() {
writeln("Hello, World!");
  writeln("Hello, World!");
}

// This has consistent indentation which follows the rule.
proc foo() {
  writeln("Hello, World!");
  writeln("Hello, World!");
}
