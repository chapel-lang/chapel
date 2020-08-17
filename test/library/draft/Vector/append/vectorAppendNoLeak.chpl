/*
  This test mirrors a similar test written for array.push_back, now rewritten
  to use vector.append instead.
*/

use Vector;

var wrappedvectors: vector(Wrappedvector);

const n = 10; // n is parsed from a file

extend(wrappedvectors, n);

for i in 0..#n do
  wrappedvectors[i].append(i);


record Wrappedvector {
  var Vector: vector(int);

  proc append(node) {
    this.Vector.append(node);
  }
}

proc extend(ref L: vector(?t, ?), n: int) {
  var default: t;
  for 1..n do
    L.append(default);
}


