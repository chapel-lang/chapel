// This is a slimmed-down version of test-random.chpl
// for ease of reading.

use MyRandom;

interface RandomStream(Stream) {
  type eltType;
  eltType implements StdOps;  // chpl__initCopy, =, ==, write()

  proc Stream.seed: int(64);
  proc Stream.getNext(): eltType;
  proc Stream.skipToNth(n: int): void throws;
  proc Stream.getNth(n: int): eltType throws;
}

implements RandomStream(borrowed PCGRandomStream(?));
implements RandomStream(borrowed PCGRandomStream(int,true));

proc testGetNth(rs1: ?Q, rs2: Q) where Q implements RandomStream {
  for i in 0..9 {
    const r1 = rs1.getNext();
    const r2 = try! rs2.getNth(i);
    if r1 != r2 then writeln("mismatch 1 at ", i);
  }
  writeln("testGetNth() done");
}

const rs1 = createRandomStream(int, 123).borrow(),
      rs2 = createRandomStream(int, 123).borrow();
testGetNth(rs1, rs2);

/////////////////////////////////

proc fillRandom(stream: PCGRandomStream(?), arr)
  // where arr implements Array1d
{
  forall (x, r) in zip(arr, stream.iterate(arr.domain, arr.eltType)) do
    x = r;
}

proc fillRandom(stream: ?Stream, arr: ?ARR)
  where Stream implements PCGRandomStreamImpl &&
        ARR implements Array1d(Stream.eltType)
        // && Stream.eltType == ARR.eltType
{
  var cursor = stream.startCursor(arr.domain);
  for i in arr.domain do
    arr[i] = stream.nextCursor(cursor);
}

/////////////////////////////////

interface PCGRandomStreamImpl(PCGstream) {
  type eltType;
  eltType implements StdOps;

  type cursorType;
  proc chpl__initCopy(arg: cursorType, definedConst: bool): cursorType;

  proc PCGstream.startCursor(D: domain(1)): cursorType;
  proc PCGstream.nextCursor(ref cursor: cursorType): eltType;
}

interface Array1d(ARR, eltType) {
  // type eltType;
  proc const ref ARR._dom: domain(1);
  proc ARR.this(idx: int) ref : eltType;
}

/////////////////////////////////

implements PCGRandomStreamImpl(borrowed PCGRandomStream(?));

var A1, A2: [0..9] int;
implements Array1d(A1.type, A1.eltType);

try! rs1.skipToNth(321);
rs1.fillRandom(A1);

try! rs2.skipToNth(321);
fillRandom(rs2, A2);

if && reduce (A1 == A2) then writeln("fillRandom() check passed");

/////////////////////////////////

proc PCGRandomStream.startCursor(D: domain) {
  type resultType = eltType;
  _lock();
  const start = PCGRandomStreamPrivate_count;
  PCGRandomStreamPrivate_count += D.size.safeCast(int(64));
  PCGRandomStreamPrivate_skipToNth_noLock(PCGRandomStreamPrivate_count-1);
  _unlock();
  return randlc_skipto(resultType, seed, start);
}

proc PCGRandomStream.nextCursor(ref cursor) {
  type resultType = eltType;
  return randlc(resultType, cursor);
}

private proc PCGcursorType type {
  var PCGstream = new PCGRandomStream(int);
  var D: domain(1);
  return PCGstream.startCursor(D).type;
}

proc PCGRandomStream.cursorType type
  return PCGcursorType;

/////////////////////////////////

interface StdOps(Val) {
  proc chpl__initCopy(arg: Val, definedConst: bool): Val;
  operator =(ref lhs: Val, rhs: Val): void;
  operator ==(lhs: Val, rhs: Val): bool;
  operator !=(lhs: Val, rhs: Val): bool return !(lhs == rhs);
  proc write(arg: Val): void;
}

bool implements StdOps;
numeric implements StdOps;
string  implements StdOps;
