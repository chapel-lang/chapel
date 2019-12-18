private use List;

var ops: list((int, unmanaged object?, int, int, int));
var opsLock$: sync bool = true;

var counter: atomic int;

require "gdb.h";
config const breakOnAllocateId = -1;

proc trackAllocation(c: object, id:int, x:int) {
  opsLock$.readFE();
  ops.append( (1, c:unmanaged, id, x, 1+counter.fetchAdd(1)) );
  if id == breakOnAllocateId {
    extern proc gdbShouldBreakHere();
    gdbShouldBreakHere();
  }
  opsLock$.writeEF(true);
}

proc trackFree(c: object, id:int, x:int) {
  opsLock$.readFE();
  ops.append( (-1, c:unmanaged, id, x, 1+counter.fetchAdd(1)) );
  opsLock$.writeEF(true);
}

proc checkAllocations() {
  use Map;
  // check that everything that was freed was allocated
  // and thet everything that was allocated was freed.
  // check that the order was OK too.

  var allocated = new map(int, int); // id to op #
  var freed = new map(int, int); // id to op #
  var to_x = new map(int, int);

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
    for id in arr.valuesToArray().sorted() {
      write("(id=", id, " x=", to_x[id], ") ");
    }
  }

  if (allocated ^ freed).size > 0 {
    writeln("alloc != free - possibly a memory leak");

    write("allocated and not freed: ");
    var alloc_not_freed = allocated - freed;
    printthem(alloc_not_freed);
    writeln();

    write("freed but not allocated: ");
    var freed_not_allocated = freed - allocated;
    printthem(freed_not_allocated);
    writeln();

    write("allocated and then freed: ");
    var alloc_freed = freed & allocated;
    printthem(alloc_freed);
    writeln();

    return false;
  }

  // check order of each id. Each id should be freed after being allocated.
  for id in allocated {
    // same as ids in freed_byid by this point.
    if allocated[id] < freed[id] {
      // OK
    } else {
      writeln("id ", id, " was allocated after being freed!");
      return false;
    }
  }
  return true;
}

