use ParallelIO, Map, IO, Math, Time;

config const fileName = "million_entries.txt",
             nTasks = 2,
             printOutput = true,
             timeExecution = false;

// how often to flush local maps to the global map
// (should be larger (~32) for a real problem size)
config const flushThreshold = 4;

proc main() {
    var t = new stopwatch();
    if timeExecution then t.start();

    var globalTempsMap = new sharedLock(new map(bytes, tempData));

    forall ct in readDelimited(fileName, t=cityTemp, delim=b"\n", nTasks=nTasks)
        with (var agg = new accumAggregator(globalTempsMap))
            do agg.addOrUpdate(ct.city, ct.temp);

    if timeExecution then writeln("elapsed time: ", t.elapsed());

    if printOutput {
        use Sort;
        var x = globalTempsMap.item.toArray();
        sort(x);
        for (city, temps) in x do writef("%20s: %?", city.decode(), temps);
    }
}

// record to store temperature stats for a particular city
record tempData: writeSerializable {
    var min: real;
    var max: real;
    var total: real;
    var count: int;
}

inline operator tempData.+=(ref td: tempData, temp: real) {
    td.min = Math.min(td.min, temp);
    td.max = Math.max(td.max, temp);
    td.total += temp;
    td.count += 1;
}

inline operator tempData.+=(ref td: tempData, tdOther: tempData) {
    td.min = Math.min(td.min, tdOther.min);
    td.max = Math.max(td.max, tdOther.max);
    td.total += tdOther.total;
    td.count += tdOther.count;
}

proc tempData.serialize(writer: fileWriter(?), ref serializer) throws {
    writer.writef("%7.1dr %7.1dr %7.1dr\n",
       this.min, this.total / this.count, this.max);
}

// a (city, temperature) pair
record cityTemp: readDeserializable {
    var city: bytes;
    var temp: real;
}

proc ref cityTemp.deserialize(reader: fileReader(?), ref deserializer) throws {
    this.city = reader.readThrough(b";", stripSeparator=true);
    this.temp = reader.read(real);
}

// aggregator to facilitate task-local accumulation of temperature stats,
// with occasional flushing to the global map (using a lock for synchronization)
record accumAggregator {
    // pointer to a lock around the global map
    var globalMap: borrowed sharedLock(map(bytes, tempData));

    // task-local map for aggregation
    var localTempsMap: map(bytes, tempData);

    proc init(globalMap: borrowed sharedLock(map(bytes, tempData))) {
        this.globalMap = globalMap;
        this.localTempsMap = new map(bytes, tempData, initialCapacity=flushThreshold);
    }

    proc ref deinit() do this.flush();

    proc ref flush() {
        // acquire the lock around the global map
        manage this.globalMap as gm {
            // merge the local map into the global map
            for (k, v) in zip(this.localTempsMap.keys(), this.localTempsMap.values()) {
                if gm.contains(k)
                    then gm[k] += v;
                    else gm[k] = v;
            }
        }

        // clear the local map
        this.localTempsMap.clear();
    }

    proc ref addOrUpdate(city: bytes, temp: real) {
        if this.localTempsMap.contains(city) {
            this.localTempsMap[city] += temp;
        } else {
            this.localTempsMap[city] = new tempData(temp, temp, temp, 1);
        }
        if this.localTempsMap.size >= flushThreshold then this.flush();
    }
}

import ChapelLocks;
class sharedLock : contextManager {
    type T;
    var lock: ChapelLocks.chpl_LocalSpinlock;
    var item: T;

    proc init(in item: ?t) {
        this.T = t;
        this.lock = new ChapelLocks.chpl_LocalSpinlock();
        this.item = item;
    }

    proc enterContext() ref: T {
        this.lock.lock();
        return this.item;
    }

    proc exitContext(in err: owned Error?) {
        if err then halt(err!.message());
        this.lock.unlock();
    }
}

