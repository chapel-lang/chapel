var ops:[1..0] (int,object,int, int, int);

var counter: atomic int;

proc trackAllocation(c: object, id:int, x:int) {
  ops.push_back( (1, c, id, x, 1+counter.fetchAdd(1)) );
}

proc trackFree(c: object, id:int, x:int) {
  ops.push_back( (-1, c, id, x, 1+counter.fetchAdd(1)) );
}

proc checkAllocations() {
  // check that everything that was freed was allocated
  // and thet everything that was allocated was freed.
  // check that the order was OK too.

  var alloc_byid:domain(int);
  var free_byid:domain(int);
  var x_byid:domain(int);
  var allocated:[alloc_byid] int; // id to op #
  var freed:[free_byid] int; // id to op #
  var to_x:[x_byid] int;

  for op in ops {
    var (optype, obj, id, x, counter) = op;
    if optype > 0 {
      allocated[id] = counter;
      to_x[id] = x;
    }
    if optype < 0 {
      freed[id] = counter;
      to_x[id] = x;
    }

  }

  proc printthem(arr)
  {
    for id in arr.sorted() {
      write("(id=", id, " x=", to_x[id], ") ");
    }
  }

  if alloc_byid != free_byid {
    writeln("alloc != free - possibly a memory leak");

    write("allocated and not freed: ");
    var alloc_not_freed = alloc_byid - free_byid;
    printthem(alloc_not_freed);
    writeln();

    write("freed but not allocated: ");
    var freed_not_allocated = free_byid - alloc_byid;
    printthem(freed_not_allocated);
    writeln();

    write("allocated and then freed: ");
    var alloc_freed = free_byid & alloc_byid;
    printthem(alloc_freed);
    writeln();

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

