writeln();

use BlockDist;

var dom = newBlockDom(0..10);

var a: [dom] int, b: [dom] int;

for i in a.domain {
  a[i] = i;
}

// we want the follower's index to be recognized as local, and have destination
// aggregation to fire
writeln("Loop 1 -- expecting destination aggregation");
forall (i, elem) in zip(a.domain, a) {
  b[10-i] = elem;
}
writeln("End Loop 1");

writeln(b);
writeln();

// this is just to make sure that the same optimization is thwarted very early
// in compilation because the statement not being the last one in the body
var dummy = 0;
writeln("Loop 2 -- expecting no aggregation");
forall (i, elem) in zip(a.domain, a) with (ref dummy) {
  b[10-i] = elem;
  dummy = b[10-i];
}
writeln("End Loop 2");

writeln(b);
writeln();

// scatter-like pattern
var randomIndices = [i in 0..10 by -1] i;
writeln("Loop 3 -- expecting destination aggregation");
forall (idx, elem) in zip(dom, a) {
  b[randomIndices[idx]] = elem;  // expect two confirmations: fast follower, slow follower
}
writeln("End Loop 3");

writeln(b);
writeln();

// gather-like pattern
writeln("Loop 4 -- expecting source aggregation");
forall (idx, elem) in zip(dom, a) {
  elem = b[randomIndices[idx]];  // expect two confirmations: fast follower, slow follower
}
writeln("End Loop 4");

writeln(a);
writeln();


// inspired by arkouda -- rhs is local but not leader, lhs is random access
writeln("Loop 5 -- expecting destination aggregation");
var otherArr: [dom] int;
forall (aElem, bElem) in zip(a, b) {
  otherArr[aElem] = bElem;
}
writeln("End Loop 5");

writeln(otherArr);
writeln();
