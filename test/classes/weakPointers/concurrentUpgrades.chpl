
use WeakPointer;
use Barriers;
import Random;

class basicClass {
    var x :int;
}

config param verbose = false;

// small scale test:
{
    var someSharedObjects = [
        new shared basicClass(0),
        new shared basicClass(1),
        new shared basicClass(2),
        new shared basicClass(3),
    ];

    var someWeakPointers = [
        new weakPointer(someSharedObjects[0]),
        new weakPointer(someSharedObjects[1]),
        new weakPointer(someSharedObjects[2]),
        new weakPointer(someSharedObjects[3]),
    ];

    var correct: atomic bool = true;

    // during the 0th iteration, upgrade 0 twice (concurrently).
        // during the next iteration, upgrade 1 twice, etc.
    for (i, tids) in [
        (0, [0, 0, 1, 2, 3]),
        (1, [0, 1, 1, 2, 3]),
        (2, [0, 1, 2, 2, 3]),
        (3, [0, 1, 2, 3, 3]),
    ] {
        var b = new Barrier(tids.size);
        coforall tid in tids {
            // try to upgrade to a strong pointer
            var sp = someWeakPointers[tid] : shared basicClass?;

            // wait for all the upgrades to finish
            b.barrier();

            // for the duplicate values, we should have 3 strong pointers and 1 weak
            // for the others, we should have 2 strong pointers and 1 weak
            const sc = someWeakPointers[tid].getStrongCount();
            const wc = someWeakPointers[tid].getWeakCount();

            const other_strong_count = sp.chpl_pn!.strongCount.read();

            if verbose then writeln(tid, "\tsc:", sc, "\twc:", wc, "\tosc: ", other_strong_count);

            if i == tid {
                correct.write(correct.read() && sc == 3);
            } else {
                correct.write(correct.read() && sc == 2);
            }
            correct.write(correct.read() && wc == 1);

            b.barrier();
        }
        if verbose then writeln("-------------------------------");
    }

    writeln(correct.read());
}


// larger scale test:
{
    const n = 15;

    var nSharedObjects: [0..<n] shared basicClass?;
    var nWeakPointers: [0..<n] weakPointer(shared basicClass?);

    for i in 0..<n {
        nSharedObjects[i] = new shared basicClass(i);
        nWeakPointers[i] = new weakPointer(nSharedObjects[i]);
    }

    var reallyCorrect: atomic bool = true;

    for i in 0..<15 {
        // create task ids with the ith id duplicated 10 times
        var tids: [0..<(n+10)] int;
        for id in 0..<n do tids[id] = id;
        for id in n..<(n+10) do tids[id] = i;

        // iterate over them concurrently in a random order
        Random.shuffle(tids);
        var b = new Barrier(n+10);

        coforall tid in tids {
            // upgrade
            var sp = nWeakPointers[tid] : shared basicClass?;

            b.barrier();

            //create another weak pointer by copy initialization
            var otherWeak = nWeakPointers[tid];

            b.barrier();

            const sc = nWeakPointers[tid].getStrongCount();
            const wc = nWeakPointers[tid].getWeakCount();
            if verbose then writeln(tid, "\tsc:", sc, "\twc:", wc);

            if i == tid {
                reallyCorrect.write(reallyCorrect.read() && sc == 12);
                reallyCorrect.write(reallyCorrect.read() && wc == 12);
            } else {
                reallyCorrect.write(reallyCorrect.read() && sc == 2);
                reallyCorrect.write(reallyCorrect.read() && wc == 2);
            }

            b.barrier();
        }
        if verbose then writeln("-------------------------------");
        if !reallyCorrect.read() then break;
    }

    writeln(reallyCorrect.read());
}
