/* Distributed Map */

private use Map, IO, Aggregator;

config const defaultNumLocalMaps = 3;     // on-locale parallelism
config param debugDistributedMap = false;

// This is the user-facing type. */
/*
`distributedMap` is a distributed parallel hashtable,
implemented as `numLocalMaps` instances of `map` on each of `targetLocales`.

The key with the hash `h` is always placed on locale `h % targetLocales.size`
into the `(h / targetLocales.size) % numLocalMaps`-th `map` on that locale.

Each `map` is "parSafe". The map instances are independent. All instances
can be accessed concurrently. Alternatively for intra-locale parallelism
we can use, for example, ConcurrentMap from the ConcurrentMap package module.
*/
pragma "always RVF"
record distributedMap {
  type keyType;
  type valType;
  // privatization id
  pragma "no doc"
  const pid = -1;
  // privatized class
  forwarding _value;
  inline proc _value return chpl_getPrivatizedCopy(
                       unmanaged DistributedMapImpl(keyType, valType), pid);

  proc init(impl: DistributedMapImpl) {
    this.keyType = impl.keyType;
    this.valType = impl.valType;
    this.pid     = impl.pid;
  }
  proc init(type keyType, type valType) {
    const impl = new unmanaged DistributedMapImpl(keyType, valType);
    this.init(impl);
  }

  proc readThis(ch) throws {
    compilerError("Reading a distributedMap is not supported");
  }

  proc writeThis(ch) throws {
    _value.write(ch);
  }
} // record distributedMap

// This is the implementation class. It is privatized.
// All fields should contain local values.
pragma "no doc"
class DistributedMapImpl {
  type keyType;
  type valType;
  const targetLocales = Locales;
  const numLocalMaps  = defaultNumLocalMaps; // must be same on all locales
  var pid = -1;
  const numLocales = targetLocales.size; // cache the locale count
  var localMaps: [0..#numLocalMaps] map(keyType, valType, parSafe=true);

  // create the original instance
  proc init(type keyType, type valType) {
    this.keyType = keyType;
    this.valType = valType;
    complete();
    this.pid = _newPrivatizedClass(this);
  }
  // create a privatized instance
  proc init(original: DistributedMapImpl, targetLocales, numLocalMaps, pid) {
    this.keyType = original.keyType;
    this.valType = original.valType;
    this.targetLocales = targetLocales;
    this.numLocalMaps  = numLocalMaps;
    this.pid = pid;
  }

  // privatization helpers
  proc dsiGetPrivatizeData()
    return (targetLocales, numLocalMaps, pid);
  proc dsiPrivatize(pdata)
    return new unmanaged DistributedMapImpl(this, pdata(0), pdata(1), pdata(2));
  inline proc getPrivatizedThis()  // todo: manually forward pid?
    return chpl_getPrivatizedCopy(this.type, pid);
  proc getPrivatizedThisOn(loc: locale) {
    var result: (this.type: class?);
    on loc do result = chpl_getPrivatizedCopy(this.type, pid);
    return result!;
  }

  // writes out the entire distributedMap
  // currently reveals implementation details, for debugging
  proc write(ch) throws {
    for loc in targetLocales do {
      const localThis = getPrivatizedThisOn(loc);
      for mapIdx in 0..#numLocalMaps do
        writeln(loc, "-", mapIdx, ": ", localThis.localMaps[mapIdx]);
    }
  }

  proc size {
    var result = 0;
    coforall loc in targetLocales with (+ reduce result) do
      on loc do
        result += + reduce (for m in getPrivatizedThis().localMaps do m.size);
    return result;
  }

  proc contains(k: keyType) {
    const (locIdx, mapIdx) = keyToLocaleAndMapIdx(numLocales, numLocalMaps, k);
    var result = false;
    on targetLocales[locIdx] do
      result = getPrivatizedThis().localMaps[mapIdx].contains(k);
    return result;
  }

  iter items() {
    for loc in targetLocales do
      for map in getPrivatizedThisOn(loc).localMaps do
        for item in map.items() do
          yield item;
  }

  // returns a task-private aggregator
  proc createAggregator(updater) {
    return new Aggregator(getPrivatizedThis(), updater);
  }

  // for use by an aggregator
  proc localeIdxForKey(key: keyType) {
    return keyToLocaleAndMapIdx(numLocales, numLocalMaps, key)(0);
  }

  // for use by an aggregator
  // assumes that everything is local
  proc applyAggregatedUpdates(buffer, updater) {
    if debugDistributedMap then
      writef("applyAggregatedUpdates  %t  %t updates\n", here, buffer.size);
    coforall mapIdx in 0..#numLocalMaps {
      const numLocales = this.numLocales;
      localMaps[mapIdx].bulkUpdate(buffer, updater,
        new _mapIdxSingletonFilter(numLocales, numLocalMaps, mapIdx));
    }
  }

  // user-facing
  // requires updater.this(key, [ref] value)
  proc bulkUpdate(keysToUpdate, updater) {
    [key in keysToUpdate with (var agg = this.createAggregator(updater))] {
      agg.add(key);
    }
  }

  // user-facing
  // requires `updater.this(key, [ref] value)` returning `none`
  // `key` must be in the map already
  proc update(k: keyType, updater) throws {
    const (locIdx, mapIdx) = keyToLocaleAndMapIdx(numLocales, numLocalMaps, k);
    on targetLocales[locIdx] {
      ref map = getPrivatizedThis().localMaps[mapIdx];
      map.update(k, updater);
    }
  }

  // user-facing, returns a non-aggregating context manager
  proc updateManager(k: keyType) {
    return new distributedMapManager(this, numLocales, numLocalMaps, k);
  }

} // class DistributedMapImpl


private inline proc keyToLocaleAndMapIdx(numLocales, numLocalMaps, key) {
  const hash = chpl__defaultHashWrapper(key);
  return (hash % numLocales,
          (hash / numLocales) % numLocalMaps);
}

record _mapIdxSingletonFilter { //private
  const numLocales;
  const numLocalMaps;
  const singletonMapIdx;

  proc skip(key) {
    return keyToLocaleAndMapIdx(numLocales, numLocalMaps, key)(1)
           != singletonMapIdx;
  }
}

record _mapIdxTrivalFilter { //private
  proc skip(key) return false;
}

// produced by distributedMap.updateManager()
record distributedMapManager {
  var   client;
  const locIdx;
  const mapIdx;
  const key;

  proc init(client, numLocales, numLocalMaps, k) {
    const (locIdx, mapIdx) = keyToLocaleAndMapIdx(numLocales, numLocalMaps, k);
    this.client = client;
    this.locIdx = locIdx;
    this.mapIdx = mapIdx;
    this.key    = k;
  }

  proc enterThis() ref {
    // todo: optimize remote accesses here
    client = client.getPrivatizedThisOn(client.targetLocales[locIdx]);
    ref map = client.localMaps[mapIdx];
    map._enter();
    return map.thisInternal(key);
  }

  proc leaveThis(in err: owned Error?) throws {
    on client {
      client.localMaps[mapIdx]._leave();
    }
    if err then throw err;
  } 
}

// factor out single-element access code from 'proc ref this'
proc ref map.thisInternal(k: keyType) ref { //private
  compilerAssert(isDefaultInitializable(valType));
  
  var (_, slot) = table.findAvailableSlot(k);
  if !table.isSlotFull(slot) {
    var val: valType;
    table.fillSlot(slot, k, val);
  }

  return table.table[slot].val;
}

// an addition to map's interface
// primarily for use by an aggregator
proc map.bulkUpdate(keysToUpdate, updater, filter) {
  _enter(); defer _leave();

  for k in keysToUpdate {
    if filter.skip(k) then continue;
    updater(k, thisInternal(k));
  }
}
