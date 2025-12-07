/*
 Loop indices should be used, or omitted if not needed.
*/

// Unused index,it will be flagged by the rule.
for i in 1..10 {
  writeln("Hello");
}

// Index used this is the preferred style.
for i in 1..10 {
  writeln(i);
}

// Index omitted,this is also preferred.
for 1..10 {
  writeln("Hello");
}
