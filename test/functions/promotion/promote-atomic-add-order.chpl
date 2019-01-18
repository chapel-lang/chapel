var A:[1..10] atomic int;

A.add(1, memory_order_seq_cst);

writeln(A[1].read());
