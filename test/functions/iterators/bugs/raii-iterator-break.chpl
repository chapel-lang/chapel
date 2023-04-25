// this is a test case from issue #6912

record RAIILock {
	var lockOwner;

	proc init(lockOwner) {
		this.lockOwner = lockOwner;
		lockOwner.lock();
	}

	proc deinit() {
		lockOwner.unlock();
	}
}

class StatefulIterator {
	var _lock$ : sync bool;

	proc lock() {
		_lock$.writeEF(true);
	}

	proc unlock() {
		_lock$.readFE();
	}

	iter these() {
		var raiiLock = new RAIILock(this);
		yield 1;
	}
}

var si = new StatefulIterator();
// This is safe: RAIILock will unlock when it is reclaimed...
for i in si do ;
writeln("Iteration w/o Break; Lock Status: ", si._lock$.isFull);
for i in si do break;
writeln("Iteration w/ Break; Lock Status: ", si._lock$.isFull);
