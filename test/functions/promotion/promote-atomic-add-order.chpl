var A:[1..10] atomic int;

A.add(1, memoryOrder.seqCst);

writeln(A[1].read());
