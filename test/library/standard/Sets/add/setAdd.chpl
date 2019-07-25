use Sets;

private var rDestructorsFired = 0;
private var cDestructorsFired = 0;

record testRecord {
  var dummy: int = 0;
  proc deinit() { rDestructorsFired += 1; }
}

class testClass {
  var dummy: int = 0;
  proc deinit() { cDestructorsFired += 1; }
}

config const testIters = 8;

// Start tests in their own scope to get a tally of all destructors fired.
{
  var set1: set(int);
  var set2: set(testRecord);
  var set3: set(testClass);

  writeln(set1);
  writeln(set2);
  writeln(set3);

  proc addItems(ref set1, ref set2, ref set3) {
    for i in 1..testIters {
      set1.add(i);
      var tr = new testRecord();
      tr.dummy = i;
      set2.add(tr);
      var tc = new shared testClass();
      tc.dummy = i;
      set3.add(tc);
    }
  }

  addItems(set1, set2, set3);

  writeln("Set of int, size ", set1.size, ":");
  writeln(set1);
  writeln("Set of testRecord, size ", set2.size, ":");
  writeln(set2);
  writeln("Set of testClass, size ", set3.size, ":");
  writeln(set3);

  addItems(set1, set2, set3);

  writeln(set1);
  writeln(set2);
  writeln(set3);
}

writeln("Record destructors fired: ", rDestructorsFired);
writeln("Class destructors fired: ", cDestructorsFired);

