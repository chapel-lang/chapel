
import Map.map;
import ChapelLocks;
import Random;
use WeakPointer;
use Barriers;

class PassiveCache {
    type dataType; // assuming this type has a an initializer that takes a string
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
            var weak_pointer = this.items.getValue(key);
            var maybe_strong = weak_pointer : shared dataType?;
            if maybe_strong != nil {
                return try! (maybe_strong : shared dataType);
            } else {
                return this.buildAndSave(key);
            }
        } else {
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

// class PersistentCache {
//     type dataType;
//     var items: map(int, shared dataType);

//     proc init(type dt) {
//         this.dataType = dt;
//         this.items = new map(int, shared dt);
//     }

//     proc getOrBuild(key: int): shared dataType {
//         if this.items.contains(key) {
//            return this.items.getValue(key);
//         } else {
//             return this.buildAndSave(key);
//         }
//     }

//     proc buildAndSave(key: int): shared dataType {
//         const item = new shared dataType(key);
//         this.items.add(key, item);
//         return item;
//     }
// }

class someDataType {
    var i: int;
}

const num_cycles = 5;
const num_task_ids = 25;
const num_repeats = 10;

config param verbose = false;

proc main() {
    var pc = new PassiveCache(someDataType);

    var correct: atomic bool = true;

    // // hold on to a shared reference to the 0th item the whole time;
    // var shared_0 = pc.getOrBuild(0);

    // for i in 0..#num_cycles {
    //     var tids: [0..<(num_task_ids+num_repeats)] int;
    //     for id in 0..<num_task_ids do tids[id] = id;
    //     for id in n+1..<(n+num_repeats) do tids[id] = i;
    //     Random.shuffle(tids);

    //     coforall tid in tids {
    //         var x = pc.getOrBuild(tid);

    //         b.barrier();

    //         var wpx = new weakPointer(x);

    //         b.barrier();

    //         correct.write(if i == tid then wpx.getStrongCount() == num_repeats else wxp.getStrongCount == 2);
    //     }
    // }

    for (i, taskGroup) in [
        (0, [0, 0, 1, 2, 3]),
        (1, [0, 1, 1, 2, 3]),
        (2, [0, 1, 2, 2, 3]),
        (3, [0, 1, 2, 3, 3]),
    ]
    {
        var b = new Barrier(taskGroup.size);
        coforall tid in taskGroup {
            var x = pc.getOrBuild(tid);

            b.barrier(); // for repeated task ids, two new strong pointers should have been created by this point

            var wpx = new weakPointer(x);

            b.barrier();

            const sc = wpx.getStrongCount();
            const wc = wpx.getWeakCount();

            const sc_correct = (if i == tid then sc == 2 else sc == 1);
            const wc_correct = (if i == tid then wc == 3 else wc == 2);

            correct.write(correct.read() && sc_correct && wc_correct);

            if verbose then writeln(tid, "\tsc:", sc, "\twc:", wc);
        }
        if verbose then writeln("----------------------------");
    }

    writeln(correct.read());
}
