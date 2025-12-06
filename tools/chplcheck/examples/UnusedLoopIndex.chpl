/*
  Loop index variables should be used in the loop body.
*/

// This loop has an unused index which violates the rule.
for i in 1..10 {
  writeln("Hello");
}

// This loop uses the index which is correct.
for i in 1..10 {
  writeln(i);
}
