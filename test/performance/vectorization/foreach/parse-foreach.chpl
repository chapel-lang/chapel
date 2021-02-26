// this test should be removed once foreach is implemented

var x: int;
proc main() {
  foreach i in 1..10 { }
  foreach i in 1..10 with (ref x) { }
  foreach (i,j) in zip(1..10, 1..10) { }
  foreach (i,j) in zip(1..10, 1..10) with (ref x) { }

  foreach 1..10 { }
  foreach 1..10 with (ref x) { }
  foreach zip(1..10, 1..10) { }
  foreach zip(1..10, 1..10) with (ref x) { }

}
