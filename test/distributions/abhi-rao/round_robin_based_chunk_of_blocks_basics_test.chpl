use RangeChunk;
var block_size = 5;
var num_tasks = 4;
var stride = 3;
var positive_zero_indexed = 0..99;
var positive_one_indexed = 1..100;
var positive_zero_indexed_strided = 0..99 by stride;
var positive_one_indexed_strided = 1..100 by stride;
var negative_indexed_strided = -4..95 by stride;
var positive_indexed_negative_strided = 3..90 by -stride;
var negative_indexed_negative_strided = -6..19 by -stride;



writeln("Non strided zero indexed range: ", positive_zero_indexed, " block_size: ", block_size, " num_tasks: ", num_tasks);
for tid in 0..num_tasks - 1 {
	writeln("for tid: ", tid);
	for chunk in blockCyclicChunks(positive_zero_indexed, block_size, tid, num_tasks) {
		writeln("got: ", chunk);
	}
}

writeln("Non strided one indexed range: ", positive_one_indexed, " block_size: ", block_size, " num_tasks: ", num_tasks);
for tid in 0..num_tasks - 1 {
	writeln("for tid: ", tid);
	for chunk in blockCyclicChunks(positive_one_indexed, block_size, tid, num_tasks) {
		writeln("got: ", chunk);
	}
}

writeln("Strided zero indexed range: ", positive_zero_indexed_strided, " block_size: ", block_size, " num_tasks: ", num_tasks, " stride: ", stride);
for tid in 0..num_tasks - 1 {
	writeln("for tid: ", tid);
	for chunk in blockCyclicChunks(positive_zero_indexed_strided, block_size, tid, num_tasks) {
		writeln("got: ", chunk);
	}
}

writeln("Strided one indexed range: ", positive_one_indexed_strided, " block_size: ", block_size, " num_tasks: ", num_tasks, " stride: ", stride);
for tid in 0..num_tasks - 1 {
	writeln("for tid: ", tid);
	for chunk in blockCyclicChunks(positive_one_indexed_strided, block_size, tid, num_tasks) {
		writeln("got: ", chunk);
	}
}

writeln("Strided negative indexed range: ", negative_indexed_strided, " block_size: ", block_size, " num_tasks: ", num_tasks, " stride: ", stride);
for tid in 0..num_tasks - 1 {
	writeln("for tid: ", tid);
	for chunk in blockCyclicChunks(negative_indexed_strided, block_size, tid, num_tasks) {
		writeln("got: ", chunk);
	}
}

writeln("Positive Indexed Negative Strided range: ", positive_indexed_negative_strided, " block_size: ", block_size, " num_tasks: ", num_tasks, " stride: ", -stride);
for tid in 0..num_tasks - 1 {
	writeln("for tid: ", tid);
	for chunk in blockCyclicChunks(positive_indexed_negative_strided, block_size, tid, num_tasks) {
		writeln("got: ", chunk);
	}
}

writeln("Negative Indexed Negative Strided range: ", negative_indexed_negative_strided, " block_size: ", block_size, " num_tasks: ", num_tasks, " stride: ", -stride);
for tid in 0..num_tasks - 1 {
	writeln("for tid: ", tid);
	for chunk in blockCyclicChunks(negative_indexed_negative_strided, block_size, tid, num_tasks) {
		writeln("got: ", chunk);
	}
}
