use Sets;

config const testIters = 1;

private var rDestructorsFired = 0;
private var cOwnedDestructorsFired = 0;
private var cSharedDestructorsFired = 0;

record testRecord {
  var dummy: int = 0;
  proc deinit() { rDestructorsFired += 1; }
}

class testOwnedClass {
  var dummy: int = 0;
  proc deinit() { cOwnedDestructorsFired += 1; }
}

class testSharedClass {
  var dummy: int = 0;
  proc deinit() { cSharedDestructorsFired += 1; }
}

proc display(ref s) {
  writeln("Set of ", s.eltType:string, " size ", s.size, ":");
  writeln(s);
}

// Test proper destructor semantics for records.
{
  var s: set(testRecord);
  display(s);
  for i in 1..testIters {
    var x = new testRecord();
    x.dummy = i;
    s.add(x);
  }
  display(s);
}

writeln("Record destructors fired: ", rDestructorsFired);

// Test proper destructor semantics for owned classes.
{
  var s: set(testOwnedClass);
  display(s);
  for i in 1..testIters {
    var x = new owned testOwnedClass();
    x.dummy = i;
    s.add(x);
  }
  display(s);
}

writeln("Class destructors fired: ", cOwnedDestructorsFired);

// Test proper destructor semantics for shared classes.
{
  var s: set(testSharedClass);
  display(s);
  for i in 1..testIters {
    var x = new shared testSharedClass();
    x.dummy = i;
    s.add(x);
  }
  display(s);
}

writeln("Class destructors fired: ", cSharedDestructorsFired);
