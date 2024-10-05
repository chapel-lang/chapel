use Allocators, CTypes;

// check that points returned by bumpPtrMemPool are aligned right

class C { var x: int(16); var y: real(32); var z: real; }

proc test(param alignment) {

  var pool = new bumpPtrMemPool(10_000, alignment=alignment);
  for 1..10 {
    var p = newWithAllocator(pool, unmanaged C);
    assert(c_ptrTo(p):c_intptr % alignment:c_intptr == 0);
  }
}

proc testParallel(param alignment) {

  var pool = new bumpPtrMemPool(100_000, alignment=alignment, parSafe=true);
  coforall 1..30 {
    var p = newWithAllocator(pool, unmanaged C);
    assert(c_ptrTo(p):c_intptr % alignment:c_intptr == 0);
  }
}

test(1);
test(2);
test(4);
test(8);
test(16);
test(32);
test(64);
test(128);

testParallel(1);
testParallel(2);
testParallel(4);
testParallel(8);
testParallel(16);
testParallel(32);
testParallel(64);
testParallel(128);

