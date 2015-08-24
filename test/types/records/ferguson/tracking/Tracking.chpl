var ops:[1..0] (int,object,int, int);

var counter: atomic int;

proc trackAllocation(c: object, id:int) {
  ops.push_back( (1, c, id, 1+counter.fetchAdd(1)) );
}

proc trackFree(c: object, id:int) {
  ops.push_back( (-1, c, id, 1+counter.fetchAdd(1)) );
}

proc checkAllocations() {
  // check that everything that was freed was allocated
  // and thet everything that was allocated was freed.
  // check that the order was OK too.

  var alloc_byid:domain(int);
  var free_byid:domain(int);
  var allocated:[alloc_byid] int; // id to op #
  var freed:[free_byid] int; // id to op #

  for op in ops {
    var (optype, obj, id, counter) = op;
    if optype > 0 {
      allocated[id] = counter;
    }
    if optype < 0 {
      freed[id] = counter;
    }

  }

  if alloc_byid != free_byid {
    writeln("alloc != free - possibly a memory leak");
    var alloc_not_freed = alloc_byid - free_byid;
    writeln("allocated and not freed: ", alloc_not_freed);
    var freed_not_allocated = free_byid - alloc_byid;
    writeln("freed but not allocated: ", freed_not_allocated);
    assert(false);
  }

  // check order of each id. Each id should be freed after being allocated.
  for id in alloc_byid {
    // same as ids in freed_byid by this point.
    if allocated[id] < freed[id] {
      // OK
    } else {
      writeln("id ", id, " was allocated after being freed!");
      assert(false);
    }
  }
}

