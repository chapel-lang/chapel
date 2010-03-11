use driver_domains;
config const shift = 100;
var s1 = shift+1;
var s2 = shift+1;
var s3 = shift+1;
var s4 = shift+1;

const MySpace4 = Space4 + fill(4,shift);
const MyDom4D: domain(4) distributed Dist4D = MySpace4;

def foo(TD: domain, A: [TD] int, TA, offset) {
  var errs = 0;
  for i in [TD] do {
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

def dit() {
  const ReindexSpace4 = [min(int)+s1..#Space4.dim(1).length,
                         min(int)+s2..#Space4.dim(2).length,
                         min(int)+s3..#Space4.dim(3).length,
                         min(int)+s4..#Space4.dim(4).length];
  const ReindexDom4D: domain(4) = ReindexSpace4;
  const offset = (MySpace4.dim(1).low-ReindexSpace4.dim(1).low,
                  MySpace4.dim(2).low-ReindexSpace4.dim(2).low,
                  MySpace4.dim(3).low-ReindexSpace4.dim(3).low,
                  MySpace4.dim(4).low-ReindexSpace4.dim(4).low);
  var A: [MyDom4D] int;
  for e in A do e = next();
  writeln("reindex A: ", foo(ReindexDom4D, A, A, offset), " errors");
}

dit();
s4 = shift;
dit();
s3 = shift;
dit();
s2 = shift;
dit();
s1 = shift;
dit();
