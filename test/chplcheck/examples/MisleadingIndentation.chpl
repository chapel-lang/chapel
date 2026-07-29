/*
  Single-statement blocks should not have misleading indentation.
*/

var x = true;
// The indentation suggests both statements are inside the if, but only the first is.
if x then
  writeln("First");
  writeln("Second");

// Indentation adjusted to reflect the actual control flow.
if x then
  writeln("First");
writeln("Second");

// Or use braces to include both statements in the block.
if x {
  writeln("First");
  writeln("Second");
}

// The indentation suggests both statements are inside the for, but only the first is.
for i in 1..3 do
  writeln("Hello");
  writeln("World");

// Indentation adjusted to reflect the actual control flow.
for i in 1..3 do
  writeln("Hello");
writeln("World");

// Or use braces.
for i in 1..3 {
  writeln("Hello");
  writeln("World");
}
