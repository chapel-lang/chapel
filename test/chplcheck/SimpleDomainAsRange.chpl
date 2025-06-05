module SimpleDomainAsRange {

  //
  // these cases should not warn
  //

  @chplcheck.ignore("UnusedLoopIndex")
  for i in 1..10 {}

  @chplcheck.ignore("UnusedLoopIndex")
  for i in 1..<10 {}

  @chplcheck.ignore("UnusedLoopIndex")
  for i in 1..#10 {}

  for 1..10 {}

  for 1..<10 {}

  for 1..#10 {}

  @chplcheck.ignore("UnusedLoopIndex")
  for (i, j) in {1..10, 1..10} {}

  @chplcheck.ignore("UnusedLoopIndex")
  for {1..10, 1..10} {}

  var r: 2*range(int);
  @chplcheck.ignore("UnusedLoopIndex")
  for i in {(...r)} {}

  @chplcheck.ignore("UnusedLoopIndex")
  for {(...r)} {}

  //
  // These cases should warn
  //

  @chplcheck.ignore("UnusedLoopIndex")
  for i in {1..10} {}

  @chplcheck.ignore("UnusedLoopIndex")
  for i in {1..<10} {}

  @chplcheck.ignore("UnusedLoopIndex")
  for i in {1..#10} {}

  for {1..10} {}

  for {1..<10} {}

  for {1..#10} {}

  for {1..#10 by 2} {}

  for {1..#10 align 2} {}

  for {1..#10 by 2 align 2} {}

  var A: [1..10] int;

  @chplcheck.ignore("UnusedLoopIndex")
  forall i in {1..#10 by 2 align 2} with (ref A) {
    A;
  }

  forall {1..10} with (ref A) {
    A;
  }

  forall {1..<10} with (ref A) {
    A;
  }

  foreach {1..#10 by 2 align 2} with (ref A) {
    A;
  }

  foreach {1..10, 1..10 by 2} {}
}
