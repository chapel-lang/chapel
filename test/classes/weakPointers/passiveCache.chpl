
import Map.map;
import ChapelLocks;
import Random;
use WeakPointer;
use Barriers;

class PassiveCache {
    type dataType; // assuming this type has a an initializer that takes an int
    var items: map(int, weakPointer(shared dataType));
    var lock$: ChapelLocks.chpl_LocalSpinlock;

    proc init(type dt) {
        this.dataType = dt;
        this.items = new map(int, weakPointer(shared dt), parSafe=true);
    }

    inline proc lock() {
      this.lock$.lock();
    }

    inline proc unlock() {
      this.lock$.unlock();
    }

    proc getOrBuild(key: int) : shared dataType {
        lock(); defer unlock();

        if this.items.contains(key) {
            // writeln("have weak: ", key);
            var weak_pointer = this.items.getValue(key);
            var maybe_strong = weak_pointer : shared dataType?;
            if maybe_strong != nil {
                // writeln("\t upgraded: ", key);
                return try! (maybe_strong : shared dataType);
            } else {
                // writeln("\t reconstructing: ", key);
                return this.buildAndSave(key);
            }
        } else {
            // writeln("first time: ", key);
            return this.buildAndSave(key);
        }
    }

    proc buildAndSave(key: int) : shared dataType {
        const item = new shared dataType(key);
        const weak_ptr = new weakPointer(item);
        this.items.addOrSet(key, weak_ptr);
        return item;
    }
}

class basicClass {
    var x: int;
}

config const num_cycles = 10;
config const num_task_ids = 15;

proc main() {
    var pc = new PassiveCache(basicClass);
    var correct: atomic bool = true;

    // hold on to an extra shared reference to the 0th for the whole test
    var shared_0 = pc.getOrBuild(0);

    for i in 0..#num_cycles {

        // create 'num_task_ids' random numbers in the range 0..<num_task_ids
        var task_ids : [0..<num_task_ids] uint;
        Random.fillRandom(task_ids);
        for tid in task_ids {
            tid %= num_task_ids;
        }

        // concurrently construct or upgrade the 'basicClass' associated with each 'tid'
        //  and ensure that the strong count is correct
        var b = new Barrier(num_task_ids);
        coforall tid in task_ids {
            var shared_tid : shared basicClass = pc.getOrBuild(tid:int);
            correct.write(correct.read() && shared_tid.x == tid);

            b.barrier();

            const this_tid_count = + reduce (task_ids == tid);
            const sc = shared_tid.chpl_pn!.strongCount.read();
            // writeln(tid, "\tsc: ", sc, "\texpected: ", this_tid_count);

            correct.write(
                correct.read() &&
                sc == this_tid_count + (if tid == 0 then 1 else 0)
            );

            b.barrier();
        }
    }

    writeln(correct.read());
}
