use DynamicIters;

proc checkRangeType(type t) {
  const r = (1:t)..(20:t);
  var A: [r] atomic int;
  for i in dynamic(r, chunkSize=5) {
    A[i].add(1);
  }
  writeln(A);
  forall i in dynamic(r, chunkSize=5) {
    A[i].add(1);
  }
  writeln(A);
  forall (i,j) in zip(dynamic(r, chunkSize=5), r) {
    assert(i == j);
    A[i].add(1);
  }
  writeln(A);
  forall (i,j) in zip(r, dynamic(r, chunkSize=5)) {
    assert(i == j);
    A[i].add(1);
  }
  writeln(A);
}


checkRangeType(int(64));
checkRangeType(int(32));
checkRangeType(int(16));
checkRangeType(int(8));

checkRangeType(uint(64));
checkRangeType(uint(32));
checkRangeType(uint(16));
checkRangeType(uint(8));

