
import Map.map;
use WeakPointer;
use Barriers;

class PassiveCache {
    type dataType; // assuming this type has a an initializer that takes a single int
    var items: map(int, weakPointer(shared dataType));

    proc init(type dt) {
        this.dataType = dt;
        this.items = new map(int, weakPointer(shared dt), true);
    }

    proc getOrBuild(key: int) : shared dataType {
        if this.items.contains(key) {
            // we've computed a value for this key before, try to get a strong pointer to it
            var weak_pointer = this.items.getValue(key);

            // if the pointer can be upgraded, return the shared reference, otherwise recompute the item and return it
            var maybe_strong = weak_pointer : shared dataType?;
            if maybe_strong != nil {
                return try! (maybe_strong : shared dataType);
            } else {
                writeln("recomputing: ", key);
                return this.buildAndSave(key);
            }

            // --- Alternative interfaces for the same behavior ---
            // return if weak_pointer.canUpgrade() then weak_pointer.forceUpgrade() else this.buildAndSave(key);
            // return if weak_pointer.canUpgrade() then (try! weak_pointer.tryUpgrade()) else this.buildAndSave(key);
        } else {
            // we haven't seen this key before; compute the item and return it
            writeln("computing for first time: ", key);
            return this.buildAndSave(key);
        }
    }

    proc buildAndSave(key: int) : shared dataType {
        // make the 'dataType' that corresponds to this 'key'
        const item = new shared dataType(key);

        // create and store a weakPointer to the shared object
        const weak_ptr = new weakPointer(item);
        this.items.add(key, weak_ptr);

        // return the shared pointer to the object
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

proc main() {
    var pc = new PassiveCache(someDataType);

    const targetStrongCounts = [
        [2, 1, 1],
        [1, 2, 1],
        [1, 1, 2]
    ];
    const targetWeakCounts = [
        [3, 2, 2],
        [2, 3, 2],
        [2, 2, 3]
    ];

    var correct: atomic bool = true;

    for (i, taskGroup) in [
        (0, [0, 0, 1, 2]),
        (1, [0, 1, 1, 2]),
        (2, [0, 1, 2, 2]),
    ]
    {
        var b = new Barrier(taskGroup.size);
        coforall tid in taskGroup {
            var x = pc.getOrBuild(tid);

            b.barrier(); // for repeated task ids, two strong pointers should have been created by this point

            var wpx = new weakPointer(x);

            b.barrier();

            const sc = wpx.getStrongCount();
            const wc = wpx.getWeakCount();

            const strongCountCorrect = targetStrongCounts[i][x.i] == sc;
            const weakCountCorrect = targetWeakCounts[i][x.i] == wc;

            writeln(i, "\t", tid, "\tsc: ", sc, "\twc:", wc);

            correct.write(strongCountCorrect && weakCountCorrect);
        }
        writeln("--------------------------");
    }

    writeln(correct.read());
}
