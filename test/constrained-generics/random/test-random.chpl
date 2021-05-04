/*
Contents:

module StandardInterfaces
  interface StdOps
  its implementations for some primitive types
  code to test the above

  interface Array1d
  its implementation for a particular array type
  ... which serves as a test

module RandomInterface
  interface RandomStream
  its implementation for borrowed PCGRandomStream

  interface PCGRandomStreamImpl
  proc icFillRandom - an IC function with PCGRandomStreamImpl and Array1d args
  implementation for borrowed PCGRandomStream

module User
  proc testGetNth()
    a test of RandomStream.getNext(), .getNth(), .skipToNth()
  proc main()
    calls testGetNth()
    a test for icFillRandom()

Many of the comments indicate desired improvements.
*/

// Once we solidify their contents, the interfaces in 'StandardInterfaces'
// should be moved the standard/internal modules.
module StandardInterfaces {

  /////////////////////////////////////////////////////////////////////
  // StdOps defines operations expected on most types
  // Todo design: do we want all interface formals and associated types
  // to implement this by default, unless explicitly opted out, ex.
  // with 'Val implements NoStandardOps' ?

  interface StdOps(Val) {
    // todo: instead do we want: proc Val.init=(other: Val) ?
    proc chpl__initCopy(arg: Val, definedConst: bool): Val;
    operator =(ref lhs: Val, rhs: Val): void;
    operator ==(lhs: Val, rhs: Val): bool;
    operator !=(lhs: Val, rhs: Val): bool return !(lhs == rhs);
    proc write(arg: Val): void;
    proc stdTypeString(arg: Val): string;
  }

  bool implements StdOps;
/*// currently does not work because: == is implemented for args of the type
  // "bool"; need coercions to call it; coercions are not supported
  chpl_anybool implements StdOps;
*/
  numeric implements StdOps;
  string  implements StdOps;
  private proc stdTypeString(arg) return arg.type :string;

/*// currently does not work because: == is implemented for args of the type
  // "borrowed object?"; need coercions to call it; coercions are not supported
  implements StdOps(borrowed class);
  implements StdOps(unmanaged class);
  implements StdOps(borrowed class?);
  implements StdOps(unmanaged class?);
*/
  // Can we declare "implements StdOps" for owned and/or shared types?
  // How to declare "implements StdOps" for POD records?
  // Any other types we can declare "implements StdOps" for?

  // this confirms that the 'implements StdOps' statements work
  private proc testOps(arg: ?Q) where Q implements StdOps {
    write("testOps "); write(arg); writeln(" :", stdTypeString(arg));
  }

  testOps(true);
  testOps(5i);
  testOps("STRING");

  /////////////////////////////////////////////////////////////////////
  // Array1d defines operations on 1-d rectangular arrays.
  // TODO: implement equality constraints so that eltType can be an assoc.type
  // TODO: need a generic implements statement that covers all 1d arrays

  interface Array1d(ARR, eltType) {
    proc const ref ARR._dom: domain(1);
    proc ARR.this(idx: int) ref : eltType;
  }

  // this confirms that an 'implements Array1d' statement works
  private var testArray: [1..10] int;
  implements Array1d(testArray.type, testArray.eltType);
  // _array implements Array1d;

} // module StandardInterfaces

module RandomInterface {
  use StandardInterfaces;

  /////////// interface RandomStream ///////////

  // Public random stream interface
  interface RandomStream(Stream) {
    type eltType;  // todo: real(64) by default?
    eltType implements StdOps;

    //todo: param parSafe: bool = true;

    proc Stream.seed: int(64);

    proc Stream.getNext(): eltType;

    // Cf. originally:  proc skipToNth(n: integral) throws
    // 
    // Using an 'Integral' interface is comlicated here.
    // Also n: uint presently does not work, see #17568.
    // So for simplicity provide just an 'int' version.
    proc Stream.skipToNth(n: int): void throws;

    // Ditto re 'integral'.
    proc Stream.getNth(n: int): eltType throws;
  }

  /////////// PCGRandomStream implements RandomStream ///////////

  import MyRandom.PCGRandomStream;
  implements RandomStream(borrowed PCGRandomStream(?));
  // how about other management strategies?

  /// We need the following adjustments to existing code.

  // Random module's PCGRandomStream.getNext takes a default argument,
  // which is currently not implemented. So instead provide this adaptor:
  proc PCGRandomStream.getNext(): eltType {
    return getNext(eltType);
  }

  /////////// fillRandom -> icFillRandom ///////////

  // simulates an IC-generic PCGRandomStream class
  interface PCGRandomStreamImpl(PCGstream) {
    type eltType;
    type cursorType;
    eltType implements StdOps;
//  cursorType implements StdOps;
    proc chpl__initCopy(arg: cursorType, definedConst: bool): cursorType;

    proc PCGstream.startCursor(D: domain(1)): cursorType;
    proc PCGstream.nextCursor(ref cursor: cursorType): eltType;
  }

  // a simplified IC version of fillRandom()
  proc icFillRandom(stream: ?Stream, arr: ?ARR)
    where Stream implements PCGRandomStreamImpl &&
          ARR implements Array1d(stream.eltType)
    // would also like: Stream.eltType == ARR.eltType
  {
    var cursor = stream.startCursor(arr.domain);
    for i in arr.domain do
      arr[i] = stream.nextCursor(cursor);
  }

  // a concrete implements statement - for testing
  implements PCGRandomStreamImpl(borrowed PCGRandomStream(int, true));

  // a generic implements statement - for complete coverage
  implements PCGRandomStreamImpl(borrowed PCGRandomStream(?));
  // how about other management strategies?

  proc PCGRandomStream.startCursor(D: domain) {
    import MyRandom.PCGRandom.randlc_skipto;
    type resultType = eltType;
    _lock();
    const start = PCGRandomStreamPrivate_count;
    PCGRandomStreamPrivate_count += D.size.safeCast(int(64));
    PCGRandomStreamPrivate_skipToNth_noLock(PCGRandomStreamPrivate_count-1);
    _unlock();
    return randlc_skipto(resultType, seed, start);
  }

  proc PCGRandomStream.nextCursor(ref cursor) {
    import MyRandom.PCGRandom.randlc;
    type resultType = eltType;
    return randlc(resultType, cursor);
  }

  private proc PCGcursorType type {
    var PCGstream = new PCGRandomStream(int);
    var D: domain(1);
    return PCGstream.startCursor(D).type; // hopefully no runtime types
  }

  proc PCGRandomStream.cursorType type
    return PCGcursorType;

} // module RandomInterface

module User {
  use RandomInterface, StandardInterfaces;
  config const n = 10;

  // based on test/library/standard/Random/bradc/testGetNth.chpl
  proc testGetNth(rs1: ?Q, rs2: Q, rs3: Q) where Q implements RandomStream {
    writeln("starting testGetNth()");
    for i in 0..#n {
      const r1 = rs1.getNext();
      const r2 = try! rs2.getNth(i);
//    write(i, "  "); write(r1); write("  "); write(r2); writeln();
      if r1 != r2 then writeln("mismatch 1 at ", i);
      if i > 0 {
        try! rs3.skipToNth(i);
        const r3 = rs3.getNext();
        if r1 != r3 then writeln("mismatch 3 at ", i);
      }
    }
    writeln("finished testGetNth()");
  }

  proc main {
    import MyRandom.createRandomStream;
    const rs1 = createRandomStream(int, 123).borrow(),
          rs2 = createRandomStream(int, 123).borrow(),
          rs3 = createRandomStream(int, 123).borrow();
    writeln("created three of ", rs1.type:string);
    testGetNth(rs1, rs2, rs3);

    // todo: factor out the following into an IC function
    var A1, A2: [1..n] int;
    // the reference version
    try! rs1.skipToNth(321);
    rs1.fillRandom(A1);
    // the IC version
    try! rs2.skipToNth(321);
    icFillRandom(rs2, A2);
    // verify
    if && reduce (A1 == A2) {
      writeln("fillRandom() check passed");
    } else {
      writeln("mismatch ref:  ", A1);
      writeln("vs IC version: ", A2);
    }
  }
}
