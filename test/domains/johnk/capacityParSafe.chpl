// capacityParSafe.chpl
//
// Test that calls to set capacity work correctly in parallel.
//

// Safety under parallel execution must be requested explicitly (!).
var D : domain(string, parSafe=true);

// Try to get requests to interfere with each other by launching 100
// tasks and then having them all try to update the requested capacity
// 100 times.
coforall i in 1..100 with(ref D) {
  for j in 1..100 do
    D.requestCapacity(i+j);
}

writeln("SUCCESS");
