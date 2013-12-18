/*
 * When retiring this future, we should beef it up to also test:
 *  - class
 *  - the case where there is one of (leader, follower) but not the other.
 */

record TEST {
  iter these() { yield 5; }
}
var t: TEST;
forall i in t do
  writeln(i);
