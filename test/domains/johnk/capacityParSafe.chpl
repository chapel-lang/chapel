// capacityParSafe.chpl
//
// Test that calls to set capacity work correctly in parallel.
//

// Safety under parallel execution must be requested explicitly (!).
var D : domain(string, parSafe=true);

coforall i in 1..1000 {
    D.requestCapacity(i);
}

writeln("SUCCESS");
