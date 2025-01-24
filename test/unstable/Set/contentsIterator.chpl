// Check we warn unstable for all versions of the iterator
// Lydia note: Adding because I'm only adding the unstable attribute to one
// overload because of #26590 and if we fix that before stabilizing these
// iterators, we will need to update the other ones to have the attribute

use Set;

var s = new set(string);

s.add("my");
s.add("great");
s.add("aunt");
s.add("sally");
s.add("told");
s.add("me");
s.add("to");
s.add("sleep");
s.add("in");
s.add("tomorrow");

// Check the behavior of the contents iterator when used in a serial context
for val in s.contents() {
  writeln(val);
}

var checkStandalone = new set(int, parSafe = true);
checkStandalone.add(1);
checkStandalone.add(2);
checkStandalone.add(3);
checkStandalone.add(4);
checkStandalone.add(5);
var sum: atomic int;

// Check the behavior of the contents iterator when used in a standalone context
forall val in checkStandalone.contents() {
  sum.add(val);
}

writeln(sum.read());

var LocalSet = new set(int,parSafe = true);
LocalSet.add(1);
LocalSet.add(2);
LocalSet.add(3);
LocalSet.add(4);
LocalSet.add(5);

var A : [0..4] int;
writeln(A.size, " ", LocalSet.size);
// Check the behavior of the contents iterator when used in a follower context
forall (a,b) in zip(A,LocalSet.contents(A.size)) {
    a=b;
}
writeln(A);

var LocalSet2= new set(int,parSafe = true);
LocalSet2.add(1);
LocalSet2.add(2);
LocalSet2.add(3);
LocalSet2.add(5);
LocalSet2.add(7);

var A2 : [0..4] int;
writeln(A2.size, " ", LocalSet2.size);
// Check the behavior of the contents iterator when used in a leader context
forall (b, a) in zip(LocalSet2.contents(LocalSet2.size), A2) {
    a=b;
}
writeln(A2);
