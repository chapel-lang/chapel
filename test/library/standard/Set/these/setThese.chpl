use Set;

config const testIters = 8;

record testRecord {
  var dummy: int = 0;
  proc init(dummy: int=0) { this.dummy = dummy; }
}

proc _cast(type t: testRecord, x: int) {
  return new testRecord(x);
}

proc doTest(type eltType) {
  var s1: set(eltType);

  var count = 0;

  for x in s1 do
    count += 1;

  assert(count == 0);

  var expected = 0;

  for i in 1..testIters {
    expected += i;
    var x = i:eltType;
    s1.add(x);
  }

  var total = 0;

  for x in s1 do
    if x.type == testRecord then
      total += x.dummy;
    else
      total += x;

  assert(total == expected);

  var parcount = 0;
  var lk$: sync int = 0;

  forall x in s1 with (ref parcount) {
    var rd = lk$;
    parcount += 1;
    lk$ = 0;
  }

  assert(parcount == s1.size);
}

doTest(int);
doTest(testRecord);

