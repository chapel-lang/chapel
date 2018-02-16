use Time;

extern proc chpl_qsbr_checkpoint();
config param nIterationsPerTask = 64 * 1024;

type localPtr = uint(64);

class node {
	var value : int;
	// Local pointer
	var next : atomic localPtr;
}

var head : atomic localPtr;

inline proc addr2node(addr) {
	return __primitive("cast", node, addr);
}

inline proc node2addr(node) {
	return __primitive("cast", localPtr, node);
}

proc push(val) {
	local {
		var n = new node(val);
		var h : localPtr;
		do {
			h = head.read();
			n.next.write(h);
		} while (!head.compareExchangeStrong(h, node2addr(n)));
	}
}

proc pop() : bool {
	local {
		var h : localPtr;
		var n : localPtr;
		do {
			h = head.read();
			if h == 0 then return false; 
			n = addr2node(h).next.read();
		} while (!head.compareExchangeStrong(h, n));

		extern proc chpl_qsbr_defer_deletion(c_void_ptr);
		chpl_qsbr_defer_deletion(addr2node(h) : c_void_ptr);
		return true;
	}
}

proc main() {
	coforall tid in 0 .. here.maxTaskPar {
		for i in 1 .. nIterationsPerTask {
			if i % 2 == 0 {
				push(i);
			} else {
				pop();
			}
			
			chpl_qsbr_checkpoint();
		}
	}

	while pop() do ;

	// Note: It is possible for other threads to have memory not yet reclaimed
	// in their defer lists before their task ended (due to another thread not
	// yet crossing a checkpoint to make reclamation), and in particular any data
	// deferred for deletion would not be eligible until all other threads were parked.
	// As for qthreads they park after 300K spins, we assume 1 second will be enough.
	sleep(1);

	chpl_qsbr_checkpoint();

}
