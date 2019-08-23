use Set;

config const testIters = 1;

var rDestructorsFired = 0;

record testRecord {
  var dummy: int = 0;
  proc deinit() { rDestructorsFired += 1; }
}

// Test proper destructor semantics for records.
{
  var tr = new testRecord();
  var s = new set(testRecord);
  for i in 1..testIters do
    s.add(tr);
}

assert(rDestructorsFired == (testIters + 1));

