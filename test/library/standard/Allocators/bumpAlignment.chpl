use Allocators, CTypes;

// check that points returned by bumpPtrMemPool are aligned right

class C {var x: int(16); var y: real(32); var z: real; }

proc test (param alignment) {

  var pool = new bumpPtrMemPool(1000, alignment=alignment);
  for i in 1..10 {
    var p = newWithAllocator(pool, unmanaged C);
    assert(c_ptrTo(p):int % alignment == 0);
  }
}

test(1);
test(2);
test(4);
test(8);
test(16);
test(32);

