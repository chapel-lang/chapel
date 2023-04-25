var A: [false..true] real = [1.2, 3.4];

var locIdx = max(bool);
forall i in A.domain with (min reduce locIdx) {
  if (i < locIdx) {
  }
}
writeln(locIdx);
