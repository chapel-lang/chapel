use driver_domains;
config const shift = 100;
var s1 = shift;
var s2 = shift;
var s3 = shift;
var s4 = shift;

const MySpace4 = Space4.translate(fill(4,shift));
const MyDom4D: domain(4) dmapped Dist4D = MySpace4;

proc foo(TD: domain, A: [TD] int, TA, offset) {
  var errs = 0;
  for i in TD do {
    if A[i] != TA[i+offset] {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    } else if A[i].locale != TA[i+offset].locale {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    }
  }
  return errs;
}

proc dit() {
  const ReindexSpace4 = {min(int)+s1..#Space4.dim(0).size,
                         min(int)+s2..#Space4.dim(1).size,
                         min(int)+s3..#Space4.dim(2).size,
                         min(int)+s4..#Space4.dim(3).size};
  const ReindexDom4D: domain(4) = ReindexSpace4;
  const offset = (MySpace4.dim(0).low-ReindexSpace4.dim(0).low,
                  MySpace4.dim(1).low-ReindexSpace4.dim(1).low,
                  MySpace4.dim(2).low-ReindexSpace4.dim(2).low,
                  MySpace4.dim(3).low-ReindexSpace4.dim(3).low);
  var A: [MyDom4D] int;
  for e in A do e = next();
  writeln("reindex A: ", foo(ReindexDom4D, A.reindex(ReindexDom4D), A, offset), " errors");
}

dit();
s4 = shift-1;
dit();
s3 = shift-1;
dit();
s2 = shift-1;
dit();
s1 = shift-1;
dit();
