// Regression test for https://github.com/chapel-lang/chapel/issues/14618

// Checks that there's not a problem when the previous chunk's end
// point plus the chunkSize > max(int).

// Loop body is empty so that it gets optimized out with --fast,
// otherwise the test would take way too long.

// If this test fails due to execution timeout, try running with
// -sdebugDynamicIters=true


// r and chunkSize are chosen so that one chunk will end towards the
// end of r, but adding one more chunkSize will overlow max(int).
// -s debugDynamicIters=true output appended to show the buggy behavior.

// Excercise serial, standalone, leader, and follower iterators

use DynamicIters;

var end = max(int) - 512;
var r = 0 .. end;

for i in dynamic(r, chunkSize=((1<<61) + 1024)) {}

forall i in dynamic(r, chunkSize=((1<<61) + 1024)) {}

forall i in zip(dynamic(r, chunkSize=((1<<61) + 1024)), r) { }

forall i in zip(r, dynamic(r, chunkSize=((1<<61) + 1024))) { }


/*
-sdebugDynamicDebug output from leader/follower when bug present:

Parallel dynamic Iterator. Working at tid 1 with range 0..2305843009213694975 yielded as 0..2305843009213694975
Parallel dynamic Iterator. Working at tid 0 with range 2305843009213694976..4611686018427389951 yielded as 2305843009213694976..4611686018427389951
Follower received range (0..2305843009213694975) ; shifting to 0..2305843009213694975
Follower received range (2305843009213694976..4611686018427389951) ; shifting to 2305843009213694976..4611686018427389951
Parallel dynamic Iterator. Working at tid 1 with range 4611686018427389952..6917529027641084927 yielded as 4611686018427389952..6917529027641084927
Parallel dynamic Iterator. Working at tid 0 with range 0..-6917529027641076737 yielded as 0..-6917529027641076737
Follower received range (4611686018427389952..6917529027641084927) ; shifting to 4611686018427389952..6917529027641084927
Follower received range (0..-6917529027641076737) ; shifting to 0..-6917529027641076737
Parallel dynamic Iterator. Working at tid 1 with range 0..-4611686018427381761 yielded as 0..-4611686018427381761
Parallel dynamic Iterator. Working at tid 0 with range 0..-2305843009213686785 yielded as 0..-2305843009213686785
Follower received range (0..-4611686018427381761) ; shifting to 0..-4611686018427381761
Follower received range (0..-2305843009213686785) ; shifting to 0..-2305843009213686785
Parallel dynamic Iterator. Working at tid 1 with range 0..8191 yielded as 0..8191
*/
