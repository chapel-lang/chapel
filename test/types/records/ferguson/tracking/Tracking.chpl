private use List;
private use Sort;
private use Map;

proc mapXor(a: map(?keyType, ?valueType, ?),
            b: map(keyType, valueType, ?)) {
  var newMap = new map(keyType, valueType, (a.parSafe || b.parSafe));
  
  try! {
    for k in a.keys() do
      if !b.contains(k) then newMap.add(k, a[k]);
  }
  try! {
    for k in b.keys() do
      if !a.contains(k) then newMap.add(k, b[k]);
  }
  return newMap;
}

proc mapSubtract(a: map(?keyType, ?valueType, ?),
                 b: map(keyType, valueType, ?)) {
  var newMap = new map(keyType, valueType, (a.parSafe || b.parSafe));
  
  try! {
    for ak in a.keys() {
      if !b.contains(ak) then
        newMap.add(ak, a[ak]);
    }
  }
  
  return newMap;
}

proc mapAnd(a: map(?keyType, ?valueType, ?),
            b: map(keyType, valueType, ?)) {
  var newMap = new map(keyType, valueType, (a.parSafe || b.parSafe));

  try! {
    for k in a.keys() do
      if b.contains(k) then
        newMap.add(k, a[k]);
  }

  return newMap;
}

var ops: list((int, unmanaged object?, int, int, int));
var opsLock$: sync bool = true;

var counter: atomic int;

require "gdb.h";
config const breakOnAllocateId = -1;

proc trackAllocation(c: object, id:int, x:int) {
  opsLock$.readFE();
  ops.pushBack( (1, c:unmanaged, id, x, 1+counter.fetchAdd(1)) );
  if id == breakOnAllocateId {
    extern proc gdbShouldBreakHere();
    gdbShouldBreakHere();
  }
  opsLock$.writeEF(true);
}

proc trackFree(c: object, id:int, x:int) {
  opsLock$.readFE();
  ops.pushBack( (-1, c:unmanaged, id, x, 1+counter.fetchAdd(1)) );
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
    for id in sorted(arr.keysToArray()) {
      write("(id=", id, " x=", to_x[id], ") ");
    }
  }

  if (mapXor(allocated, freed)).size > 0 {
    writeln("alloc != free - possibly a memory leak");

    write("allocated and not freed: ");
    var alloc_not_freed = mapSubtract(allocated, freed);
    printthem(alloc_not_freed);
    writeln();

    write("freed but not allocated: ");
    var freed_not_allocated = mapSubtract(freed, allocated);
    printthem(freed_not_allocated);
    writeln();

    write("allocated and then freed: ");
    var alloc_freed = mapAnd(freed, allocated);
    printthem(alloc_freed);
    writeln();

    return false;
  }

  // check order of each id. Each id should be freed after being allocated.
  for id in allocated.keys() {
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
