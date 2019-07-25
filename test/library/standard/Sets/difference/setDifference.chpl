use Sets;

record testRecord {
  var dummy: int = 0;
  proc deinit() { writeln("Destroying testRecord!"); }
}

class testClass {
  var dummy: int = 0;
  proc deinit() { writeln("Destroying testClass!"); }
}

config const testIters = 8;

var set1: set(int);
var set2: set(testRecord);
var set3: set(testClass);

writeln(set1);
writeln(set2);
writeln(set3);

proc addItems(ref set1, ref set2, ref set3, bound=testIters) {
  for i in 1..bound {
    set1.add(i);
    var tr = new testRecord();
    tr.dummy = i;
    set2.add(tr);
    var tc = new testClass();
    tc.dummy = i;
    set3.add(tc);
  }
}

addItems(set1, set2, set3);

writeln(set1);
writeln(set2);
writeln(set3);

var set4 = new set(int);
var set5 = new set(testRecord);
var set6 = new set(testClass);

addItems(set4, set5, set6, testIters*2);

// Bind the new sets to a LHS.
var set7 = set1 - set4;
var set8 = set2 - set5;
var set9 = set3 - set6;

writeln(set7);
writeln(set8);
writeln(set9);

// Now leave the next sets as unbound RHS values.
writeln(set4 - set1);
writeln(set5 - set2);
writeln(set6 - set3);

// Just to make sure...
writeln(set4 - set4);
writeln(set5 - set5);
writeln(set6 - set6);

