
use WeakPointer;
use Collectives;

class basicClass {
    var x :int;
}

config const n = 20;

proc concurrentlyUpgrade(s) {
    const wp = new weak(s);
    var correct: atomic bool = true;

    var b = new barrier(n);

    coforall tid in 0..<n {
        var my_weak = wp;

        var maybe_strong = my_weak : shared basicClass?;
        var strong = try! (maybe_strong : shared basicClass);
        correct.write(correct.read() && strong.x == 99);

        b.barrier();

        const sc = my_weak.getStrongCount();
        const wc = my_weak.getWeakCount();

        correct.write(correct.read() && sc == n+1 && wc == n+1);

        b.barrier();
    }

    writeln(correct.read());
}


var myShared = new shared basicClass(99);
concurrentlyUpgrade(myShared);

var myNilableShared : shared basicClass = new shared basicClass(99);
concurrentlyUpgrade(myNilableShared);
