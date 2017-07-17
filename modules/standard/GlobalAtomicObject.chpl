/*
  Provides a software solution to the problem of applying atomic operations using
  128-bit wide pointers via compression. The algorithm used for compression (and
  decompression) rely on the number locales. In majority of cases where numLocales
  is within a 16 bit range, we can encode it in the upper 16 bits of the address
  as only 2^48 bits of the virtual address space is ever used. In the cases where
  numLocales is between 2^17 and 2^32, we use a novel but simple technique of
  maintaining 'tables' that can be indexed into via a 32-bit integer, allowing the
  other 32-bits to be used for the locale id. The table can be indexed without
  need of synchronization, but as a Chapel array cannot be operated on while it's
  domain is being updated, we use a simple `sync` variable locally and maintain
  32 segments that are allocated on demand. While descriptors are valid, the segment
  being indexed into will be safe to use without synchronization. A simple skip list
  is utilized to efficiently hash objects to their unique index.
*/

use BlockDist;
use Random;
use BigInteger;

// In the case where you want to use the descriptor table approach for testing or
// because there's some architecture which the assumption that only 48 bits are used
// for the virtual address space is incorrect, you can disable it here.
config param usePointerCompression = false;


// We require this to create a wide pointer after decompression.
extern type wide_ptr_t;
extern type c_nodeid_t;
extern proc chpl_return_wide_ptr_node(c_nodeid_t, c_void_ptr) : wide_ptr_t;

// Hashes a class instance by pointer. We ignore the locale id portion because
// this is used for pointers that refer into our address space.
inline proc hash(obj) : uint where isClass(obj.type) {
  var key =  __primitive("cast", uint(64), __primitive("_wide_get_addr", obj));
  key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8); // key * 265
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4); // key * 21
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return key;
}

proc hash(obj) {
  halt("Obj to hash *must* be a class instance...");
}


/*
  The core `GlobalAtomicObject` which manages compression and decompression
  for atomic objects. It should be noted that if numLocales >= 2^17, then any
  object being operated on is added to the descriptor table, which will leak data
  if never freed with '_delete' (although if the memory is reused,
  so will the memory managed by this object). The descriptor table will be cleaned
  up automatically when this goes out of scope.
*/

const compressedAddrMask = 0x0000FFFFFFFFFFFF;
const compressedLocaleIdMask = 0xFFFF;
const tableLocaleIdMask = 0xFFFFFFFF;
const tableIdxMask = 0xFFFFFFFF;
const compressedLocIdOffset = 48;
const tableLocIdOffset = 32;

record GlobalAtomicObject {
  type objType;
  var _atomicVar : atomic uint(64);

  // We maintain a SkipList to manage memory (which is built on top of the descriptor
  // table itself). Currently, as the SkipList is not concurrent (requires Hazard Pointers
  // which requires thread-local storage, or task-local storage in this case).
  var perLocaleDomain = LocaleSpace dmapped Block(boundingBox=LocaleSpace);
  var descriptorTables : [perLocaleDomain] SkipList(objType);
  var descriptorTableLocks : [perLocaleDomain] sync bool;

  // Only if number of locales fits within 16 bits...
  inline proc shouldCompress {
    return usePointerCompression && (numLocales & compressedLocaleIdMask == numLocales);
  }

  inline proc getLocaleId(obj) {
    return chpl_nodeFromLocaleID(__primitive("_wide_get_locale", obj)) : uint;
  }

  inline proc getAddr(obj) {
    return __primitive("cast", uint, __primitive("_wide_get_addr", obj));
  }

  inline proc ourLocaleIdx {
    return perLocaleDomain.localSubdomain().first;
  }

  inline proc maxDescriptorTableLocaleCheck(locId) {
    if (locId & tableLocaleIdMask) != locId {
      halt("LocaleID: ", locId, " > 2^32");
    }
  }

  inline proc maxDescriptorTableIndexCheck(idx) {
    if (idx & tableIdxMask) != idx {
      halt("Idx: ", idx, " > 2^32");
    }
  }

  inline proc widePointerCheck(obj) {
    if !__primitive("is wide pointer", obj) {
      halt(
        "Dummy object created was not a wide pointer!",
        " Assumption: Inside a 'local' block"
      );
    }
  }

  inline proc uintToCVoidPtr(addr) {
    return __primitive("cast", c_void_ptr, addr);
  }

  /*
    Compresses an object into a descriptor.
  */
  proc compress(obj:objType) : uint {
    if obj == nil then return 0;

    // Perform a faster compression by packing the 48 usable bits of the virtual
    // address with 16 bits of the locale/node id.
    if shouldCompress {
      var locId = getLocaleId(obj);
      var addr = getAddr(obj);
      return (locId << compressedLocIdOffset) | (addr & compressedAddrMask);
    }

    // Fallback: We use descriptor tables
    var descriptor : uint;
    on obj {
      // Lock
      descriptorTableLocks[ourLocaleIdx] = true;

      // The skip list returns the node, which contains the table index.
      // We return a node instead of just the index for potential testing purposes.
      var node = descriptorTables[ourLocaleIdx].insert(obj);
      var locId = here.id;
      maxDescriptorTableLocaleCheck(locId);
      var idx = node.idx;
      maxDescriptorTableIndexCheck(idx);
      descriptor = locId << tableLocIdOffset | idx;

      // Unlock
      descriptorTableLocks[ourLocaleIdx];
    }

    return descriptor;
  }

  /*
    Decompresses a descriptor into the wide pointer object.
  */
  proc decompress(descr:uint) : objType {
    if descr == 0 then return nil;


    // If we have less than 2^16 locales, then we know we performed the
    // faster compression method so we need to decompress it in the same way...
    if shouldCompress {
      var locId = descr >> compressedLocIdOffset;
      var addr = descr & compressedAddrMask;
      var wideptr = chpl_return_wide_ptr_node(locId, uintToCVoidPtr(addr));

      // We've created the wide pointer, but unfortunately Chapel does not support
      // the ability to cast it to the actual object, so we have to do some
      // trickery to get it to work. What we do is we allocate a wide pointer on
      // the stack and memcpy our wideptr into the other. This is needed so we
      // have the same type.
      var newObj : objType;
      on Locales[here.id] do newObj = nil;
      widePointerCheck(newObj);
      c_memcpy(c_ptrTo(newObj), c_ptrTo(wideptr), 16);

      return newObj;
    }

    // Fallback: Descriptor table was used, and we can directly index into the
    // the skip list's memory pool.
    var locId = descr >> tableLocIdOffset;
    var idx = descr & tableIdxMask;
    return descriptorTables[locId : int].memPool.access(idx : int).key;
  }

  inline proc read() : objType {
    return decompress(_atomicVar.read());
  }

  inline proc write(obj:objType) {
    _atomicVar.write(compress(obj));
  }

  inline proc exchange(obj:objType) : objType {
    return decompress(_atomicVar.exchange(compress(obj)));
  }

  // TODO: Save time by comparing the actual objects, not just the hash... if both
  // objects are remote, this is a *significant* cost.
  //  var currDescr = _atomicVar.read();
  //  var currObj = decompress(currDescr);
  //  if currObj == expectedObj then _atomicVar.compareExchangeStrong(currDescr, compress(newObj));
  inline proc compareExchange(expectedObj:objType, newObj:objType) : bool {
    return _atomicVar.compareExchangeStrong(compress(expectedObj), compress(newObj));
  }

  /*
    Deletes the object from our table so it may be recycled.
  */
  inline proc _delete(obj:objType) {
    if obj != nil then on obj {
      descriptorTableLocks[descriptorTableLocks.domain.localSubdomain().first] = true;
      descriptorTables[descriptorTables.domain.localSubdomain().first].remove(obj);
      descriptorTableLocks[descriptorTableLocks.domain.localSubdomain().first];
    }
  }
}

/*
  Skip List implementation

  TODO: This needs to be made concurrent or else this will be the bottleneck
  for our implementation. Hazard Pointers likely needed (this also would be a
  viable example for a need of per-locale garbage collection. Perhaps make it
  possible to declare a zone such that no wide-pointers can be generated and that
  data cannot escape such a zone. It'd be very useful for lock-free/wait-free
  data structures, and should make garbage collection possible if mark-and-sweep
  stop-the-world is *only* for the task inside of the zone. Then again, hazard
  pointers work too...)
*/

param globalAtomicObjectSkipListMaxLevel = 4;

class SkipListNode {
  type keyType;
  var hash : uint(64);
  var key : keyType;
  var idx : uint;
  var forward : globalAtomicObjectSkipListMaxLevel * SkipListNode(keyType);
}

record SkipList {
  type keyType;
  var level = 1;
  var header : SkipListNode(keyType);
  var memPool : FlatObjectPool(SkipListNode(keyType));

  proc SkipList(type keyType) {
    memPool = new FlatObjectPool(SkipListNode(keyType), lambda () { return new SkipListNode(keyType); });
    header = makeNode(level);
  }

  inline proc randomLevel() {
    var levelRNG = makeRandomStream(real);
    var _level = 1;
    while levelRNG.getNext() < 0.5 && _level < globalAtomicObjectSkipListMaxLevel {
      _level = _level + 1;
    }
    return _level;
  }

  // Recycle and clean before usage...
  inline proc makeNode(_level, hash : uint = 0, key : keyType = nil) : SkipListNode(keyType) {
    var (idx, node) = memPool.alloc();
    node.idx = idx : uint;
    node.hash = hash;
    node.key = key;
    for i in 1 .. globalAtomicObjectSkipListMaxLevel do node.forward[i] = nil;

    return node;
  }

  proc remove(key : keyType) {
    // We hash the object to remove from the list. Note: We do not take into
    // account the localeId, so the object *must* not be a wide pointer.
    var hashKey = hash(key);
    var update : globalAtomicObjectSkipListMaxLevel * SkipListNode(keyType);
    var currNode = header;
    var currLevel = level;

    // Start at higher level first...
    while currLevel >= 1 {
      // Maintain same level so long ourHashKey > theirHashKey
      while currNode.forward[currLevel] != nil && currNode.forward[currLevel].hash < hashKey {
        currNode = currNode.forward[currLevel];
      }

      // At this point, ourHashKey < theirHashKey meaning we can't travel along this level.
      // However, because we are before them, that means that we would forward to them (if
      // we were to insert ourselves)
      update[currLevel] = currNode;
      currLevel = currLevel - 1;
    }

    // At this point, we cannot traverse any further than the bottom level.
    currNode = currNode.forward[1];
    if currNode == nil || currNode.hash != hashKey {
      return;
    }

    // It is a match, prepare to delete it by bridging the gap...
    for i in 1 .. level {
      if update[i].forward[i] != currNode then break;
      update[i].forward[i] = currNode.forward[i];
    }

    // Recycle node
    memPool.dealloc(currNode.idx);

    // If we are the highest node in the list, then we can reduce it.
    while level > 1 && header.forward[level] == nil {
      level = level - 1;
    }
  }

  proc insert(key : keyType) : SkipListNode(keyType) {
    // We hash the object to insert into the list. Note: We do not take into
    // account the localeId, so the object *must* not be a wide pointer.
    var hashKey = hash(key);
    var update : globalAtomicObjectSkipListMaxLevel * SkipListNode(keyType);
    var currNode = header;
    var currLevel = level;

    // Start at higher level first...
    while currLevel >= 1 {
      // Maintain same level so long ourHashKey > theirHashKey
      while currNode.forward[currLevel] != nil && currNode.forward[currLevel].hash < hashKey {
        currNode = currNode.forward[currLevel];
      }

      // At this point, ourHashKey < theirHashKey meaning we can't travel along this level.
      // However, because we are before them, that means that we would forward to them (if
      // we were to insert ourselves)
      update[currLevel] = currNode;
      currLevel = currLevel - 1;
    }

    // At this point, we cannot traverse any further than the bottom level.
    // Note that in this application, if it already exists then we can safely
    // return it.
    currNode = currNode.forward[1];
    if currNode != nil && currNode.hash == hashKey {
      return currNode;
    }

    // It doesn't exist (yet), so create one with a randomized level...
    var newLevel = randomLevel();

    // If the new level exceeds the current, we need to ensure that we are
    // forwardable from the header...
    if newLevel > level {
      for i in level + 1 .. newLevel {
        update[i] = header;
      }
      level = newLevel;
    }

    // Create new node and splice in
    currNode = makeNode(newLevel, hashKey, key);

    // Bridging the gap...
    for i in 1 .. newLevel {
      currNode.forward[i] = update[i].forward[i];
      update[i].forward[i] = currNode;
    }

    return currNode;
  }

  proc ~SkipList() {
    // Deleting memory pool ensures all memory that has been allocated with it gets
    // destroyed as well.
    delete memPool;
  }
}

/*
  Our memory pool...

  In our implementation, we keep track of 32 segments that is managed by a
  bitmap (hence the *need* for big integer). Initially, we start as empty and
  expand readily on demand without ever really contracting (I.E freeing memory)
  so once an index points to a segment, it will always refer to valid memory,
  which is crucial to allowing reads to be wait-free. Allocating is as simple
  as finding the first free bit, and using the index of that bit (*not* the value)
  to locate the segment. Recycling is as simple as clearing the bit.

  Segments are interesting in that they work extremely well with bitmaps and also
  expands at a reasonable rate. To give an example, initially the size of the first
  segment is 1 (2^1 - 1), the size of the second is 3 (2^2 - 1), the third is
  7 (2^3 - 1), of which add up to less than 2^32. Not only do we get to nearly
  double our size each time, we get to manage in-use memory in constant time,
  and an index can easily index into the table without needing to do anything.

  Also note that currently there is no bounds checking, and it is up to the user
  to ensure that no more than 2^32 objects are added.

  Potential Application: If there is a bounded number of indexes for a domain, you
  can create segments ahead of time and use a similar allocation strategy. For example,
  a 64-bit integer can use 64 segments in a similar fashion and allow concurrent
  accesses during resizing. Iteration can iterate over all segments that are in use.
  Just an idea... would work for distributions too.
*/

class FlatObjectPoolSegment {
  type objType;
  var size : int;
  var data : [{0 .. size-1}] objType;
}

class FlatObjectPool {
  type objType;
  var spawnFn : func(objType); /* Creating from generic type workaround */
  var freeFn : func(objType, void);
  var bitmap : bigint;
  var segmentSpace = {1..32};
  var segments : [segmentSpace] FlatObjectPoolSegment(objType);

  inline proc findSegment(n) {
    var bit = 31;
    while bit > 0 {
      if n & (1 << bit) != 0 then break;
      bit = bit - 1;
    }

    return bit;
  }

  inline proc findSegmentIndex(n) {
    // If n is 1, then there is only one slot
    if n == 1 {
      return 0;
    }

    var mostSignificantBit = findSegment(n);
    return n & ((1 << mostSignificantBit) - 1);
  }

  proc alloc() : (int, objType) {
    // Find an open spot, but resize if one is not found...
    var firstFreeBit = bitmap.scan0(1) : int;
    bitmap.setbit(firstFreeBit);

    var seg = findSegment(firstFreeBit) + 1;
    var segIdx = findSegmentIndex(firstFreeBit);

    // First-Touch Allocation
    if segments[seg] == nil {
        segments[seg] = new FlatObjectPoolSegment(objType, (1 << (seg - 1)));
    }

    ref obj = segments[seg].data[segIdx];

    // Reuse if already allocated, otherwise allocate and use
    if obj == nil then obj = spawnFn();
    // Get by value...
    var retval = obj;

    return (firstFreeBit, retval);
  }

  proc dealloc(idx) {
    bitmap.clrbit(idx);
  }

  // Access by index. This is safe because segments are not freed after allocation
  // and so concurrent accesses will not invalidate a valid index.
  proc access(idx) : objType {
    return segments[findSegment(idx) + 1].data[findSegmentIndex(idx)];
  }

  proc FlatObjectPool(type objType, spawnFn = nil, freeFn = nil) {
    segments[1] = new FlatObjectPool(objType, 1);
  }

  proc ~FlatObjectPool() {
    for seg in segments {
      if seg != nil {
        for obj in seg.data {
          if obj != nil {
            if freeFn != nil {
              freeFn(obj);
            } else {
              delete obj;
            }
          }
        }
      }
    }
  }
}
