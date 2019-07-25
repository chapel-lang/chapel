use Sets;

record testRecord {
  var dummy: int = 0;
}

class testClass {
  var dummy: int = 0;
}

config const testIters = 8;

// Start tests in their own scope to get a tally of all destructors fired.
var set1: set(int);
var set2: set(testRecord);
var set3: set(owned testClass);
var set4: set(shared testClass);

proc display(ref s) {
  writeln("Set of ", s.eltType:string, " of size ", s.size, ":");
  writeln(s);
}

display(set1);
display(set2);
display(set3);
display(set4);

addItems(set1, set2, set3, set4);

display(set1);
display(set2);
display(set3);
display(set4);

addItems(set1, set2, set3, set4);

display(set1);
display(set2);
display(set3);
display(set4);

