// Note, this is a duplicate of
// test/extern/bradc/structs/externFloat4calls.someFields.chpl to futurize the
// incorrect behavior without losing the normal testing value of the original.
// Once this future has been resolved, this test should be removed and the
// baseline skipif for the original removed.

extern record float4 {
  var x,y: real(32);
}

extern proc getfloat4(): float4;
extern proc printme(val: float4);

var myf42: float4 = getfloat4();
writeln("myf42 = ", myf42);
printme(myf42);
