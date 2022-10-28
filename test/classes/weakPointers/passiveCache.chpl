
import Map.map;
import ChapelLocks;
use WeakPointer;
use Barriers;

class PassiveCache {
    type dataType; // assuming this type has a an initializer that takes a string
    var items: map(string, weakPointer(shared dataType));
    var lock$: ChapelLocks.chpl_LocalSpinlock;

    proc init(type dt) {
        this.dataType = dt;
        this.items = new map(string, weakPointer(shared dt), parSafe=true);
    }

    inline proc lock() {
      this.lock$.lock();
    }

    inline proc unlock() {
      this.lock$.unlock();
    }

    proc getOrBuild(key: string) : shared dataType {
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

    proc buildAndSave(key: string) : shared dataType {
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

    proc init(s: string) {
        this.i = s:int;
    }
}

proc main() {
    var pc = new PassiveCache(someDataType);

    var correct: atomic bool = true;

    for (i, taskGroup) in [
        (0, [0, 0, 1, 2, 3]),
        (1, [0, 1, 1, 2, 3]),
        (2, [0, 1, 2, 2, 3]),
        (3, [0, 1, 2, 3, 3]),
    ]
    {
        var b = new Barrier(taskGroup.size);
        coforall tid in taskGroup {
            var x = pc.getOrBuild(tid:string);

            b.barrier(); // for repeated task ids, two new strong pointers should have been created by this point

            var wpx = new weakPointer(x);

            b.barrier();

            correct.write(if i == tid then wpx.getStrongCount() == 3 else wpx.getStrongCount() == 2);
            correct.write(wpx.getWeakCount() == 2);
        }
    }

    writeln(correct.read());
}
