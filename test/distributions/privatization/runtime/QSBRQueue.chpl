config param nIterationsPerTask = 1024;

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

proc enqueue(val) {
	var n = new node(val);
	var h : localPtr;
	do {
		h = head.read();
		n.next.write(h);
	} while (!head.compareExchangeStrong(h, node2addr(n)));
}

proc dequeue(val) {
	var h : localPtr;
	var n : localPtr;
	do {
		h = head.read();
		if h == 0 then return;
		n = addr2node(h).next.read();
	} while (!head.compareExchangeStrong(h, n));

	extern proc chpl_qsbr_defer_deletion(c_void_ptr);
	chpl_qsbr_defer_deletion(addr2node(h) : c_void_ptr);
}

proc main() {
	coforall 0 .. #here.maxTaskPar {
		for i in 1 .. nIterationsPerTask {
			if i % 2 == 0 {
				enqueue(i);
			} else {
				dequeue(i);
			}

			extern proc chpl_qsbr_checkpoint();
			chpl_qsbr_checkpoint();
		}
	} 
}
