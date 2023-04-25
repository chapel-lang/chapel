
module ChainTable {
  private use CTypes;

  const defaultInitialCapacity : uint = 16;

  const loadFactorGrowThreshold : real = 1.5,
      loadFactorShrinkThreshold: real = 0.125;

  // a hash-table implementation composed of a list of buckets (linked-lists of entries)
  record chainTable {
    type keyType;
    type valType;

    var numBuckets : uint;
    var numEntries : atomic uint;

    var numStaticManagers: atomic uint;
    var isRehashing: atomic bool;

    var bucketDom = {0..<numBuckets};
    var buckets: [bucketDom] Bucket(keyType, valType);

    proc init(type keyType, type valType, initialCapacity = defaultInitialCapacity) {
      if isDomainType(keyType) then
          compilerError("Values of 'domain' type do not support hash functions yet", 2);

      this.keyType = keyType;
      this.valType = valType;
      this.numBuckets = initialCapacity;
      this.complete();
      this.numEntries.write(0);
    }

    // determine if the map has an entry for the given key
    //  if so, return (true, bucket index, index within the bucket)
    //  otherwise, return (false, bucket index, 0)
    proc getFullSlotFor(key: keyType) : (bool, (uint, uint)) {
        var bucket_idx = this._bucketIdx(key);
        var (has_key, chain_idx) = this.buckets[bucket_idx].findIndexOf(key);
        return (has_key, (bucket_idx, chain_idx));
    }

    // return the indices for a given key
    //  if an entry is already present for the key, return (bucket index, its index within the bucket)
    //  otherwise, return (bucket index, next open index in the bucket)
    proc getSlotFor(key: keyType) : (uint, uint) {
        var bucket_idx = this._bucketIdx(key);
        var (has_key, chain_idx) = this.buckets[bucket_idx].findIndexOf(key);
        return if has_key
            then (bucket_idx, chain_idx)
            else (bucket_idx, this.buckets[bucket_idx].firstAvailableIndex());
    }

    // copy a key-value pair into a slot with the given indices
    proc fillSlot((bucket_idx, chain_idx): (uint, uint), in key: keyType, in val: valType) {
        use MemMove;

        ref entry = this.buckets[bucket_idx][chain_idx];
        if entry.status != entryStatus.full then this.numEntries.add(1);

        entry.status = entryStatus.full;
        moveInitialize(entry.key, key);
        moveInitialize(entry.val, val);
    }

    // copy a key-value pair out of a slot with the given indices,
    //  leaving the slot empty
    proc remove((bucket_idx, chain_idx): (uint, uint), out key: keyType, out val: valType) {
      use MemMove;

      ref entry = this.buckets[bucket_idx][chain_idx];

      key = moveFrom(entry.key);
      val = moveFrom(entry.val);
      entry.status = entryStatus.deleted;

      this.numEntries.sub(1);
    }


    // yield references to all the full entries in the map
    iter items() ref : rawTableEntry(this.keyType, this.valType) {
      for bucket in this.buckets do
          for entry in bucket.allEntries() do
            if entry.isFull() then
              yield entry;
    }

    // move the current entries into a new array of buckets with the given size
    proc __rehash(newNumBuckets: uint) {
      use MemMove;

      var newBucketDom = {0..<newNumBuckets},
          newBuckets : [newBucketDom] Bucket(this.keyType, this.valType);

      for bucket in this.buckets {
        for entry in bucket.allEntries() {
          if entry.isFull() {
            var new_idx = chpl__defaultHashWrapper(entry.key):uint & (newNumBuckets - 1);
            var bucket_idx = newBuckets[new_idx].firstAvailableIndex();

            ref e = newBuckets[new_idx][bucket_idx];
            e.key = entry.key;
            e.val = entry.val;
            e.status = entryStatus.full;
          }
        }
      }

      this.bucketDom = newBucketDom;
      this.buckets = newBuckets;
      this.numBuckets = newNumBuckets;
    }

    proc __incrementStaticCount() {
      this.numStaticManagers.add(1);
    }

    proc __decrementStaticCount() {
      this.numStaticManagers.sub(1);
    }

    proc __maybeResize() {
      var expectedNumManagers = this.numStaticManagers.read();
      do {
        if expectedNumManagers > 0 then return;
      } while !this.numStaticManagers.compareExchange(expectedNumManagers, 0);

      if this._loadFactor() > loadFactorGrowThreshold {
        if !isRehashing.testAndSet() {
          this.__rehash(this.numBuckets * 2);
          this.isRehashing.clear();
        }
      }
    }

    inline proc _bucketIdx(key: keyType) : uint {
      return chpl__defaultHashWrapper(key):uint & (this.numBuckets - 1);
    }

    inline proc _loadFactor() : real {
      return (this.numEntries.read() : real) / (this.numBuckets: real);
    }
  }

  // a linked-list of entries
  record Bucket {
    type keyType;
    type valType;

    var head: owned ChainTableEntry(keyType, valType)?;

    // determine if this bucket has a 'full' entry with the given key.
    //  if so, return (true, idx of the slot)
    //  otherwise, return (false, 0)
    proc findIndexOf(key: keyType) : (bool, uint) {
      for (e, idx) in zip(this.allEntries(), 0..) do
        if e.isFull() && keysMatch(e.key, key) then return (true, idx);

      return (false, 0);
    }

    // return the index of the next open slot
    // create a new slot if none are open
    proc firstAvailableIndex() : uint {
      for (e, idx) in zip(this.allEntries(), 0..) {
        if !e.isFull() then return idx;
      }

      // none of the existing slots are empty or deleted,
      //  so add one to the end of the linked list and return its index
      if this.head == nil {
        this.head = new owned ChainTableEntry(this.keyType, this.valType);
        return 0;
      } else {
        return this.head!.appendToTail(1);
      }
    }

    // return a reference to the entry at the given index
    proc this(idx: uint) ref {
      var cnt = 0,
          h = this.head.borrow();

      while h != nil {
        if cnt == idx {
          ref entry = h!.entry;
          return entry;
        }
        h = h!.next.borrow();
        cnt += 1;
      }
      halt("Bucket index out of bounds!");
    }

    // yield references to all the entries in the linked list
    iter allEntries() ref : rawTableEntry(this.keyType, this.valType) {
      var h = this.head.borrow();
      while h != nil {
        ref entry = h!.entry;
        yield entry;
        h = h!.next.borrow();
      }
    }
  }

  // A linked-list node with a key-value entry and pointer to next node
  class ChainTableEntry {
    type keyType;
    type valType;

    var entry: rawTableEntry(keyType, valType);
    var next: owned ChainTableEntry(keyType, valType)?;

    // allocate a new entry on the heap
    proc init(type keyType, type valType) {
      this.keyType = keyType;
      this.valType = valType;

      this.entry = new rawTableEntry(keyType, valType);
      this.next = nil;
    }

    // add a new entry to the tail of this linked list and return its index
    proc appendToTail(idx: int): int {
      if this.next == nil {
        this.next = new owned ChainTableEntry(keyType, valType);
        return idx;
      } else {
        return this.next!.appendToTail(idx + 1);
      }
    }
  }

  // a key-value pair
  //  when zero-initialized, this record will have an 'entryStatus' of 'empty'
  record rawTableEntry {
    type keyType;
    type valType;

    var key: keyType;
    var val: valType;

    var status: entryStatus = entryStatus.empty;
    inline proc isFull() {
      return this.status == entryStatus.full;
    }
  }

  enum entryStatus { empty=0, full, deleted };

  // ------------------- helpers -------------------

  private proc keysMatch(key1: ?t, key2: t): bool {
    if isArrayType(key2.type) {
      return (key1.equals(key2));
    } else {
      return key1 == key2;
    }
  }
}
