/* 
   NAS Parallel Benchmarks 
   "Embarrassingly Parallel"
   
   This is a Chapel implementation of the embarrassingly parallel benchmark.
   It uses the Chapel pseudo-random number generator, which is modeled after 
   the NAS linear congruential generator. Due to the different structure of 
   the Chapel language, this implementation has a noticeably different structure
   than the Fortan reference version. However, attempts were made to keep it
   aligned with the reference implementation with respect to the work that is
   done and the results that are reported. For example, while this version uses
   Chapel's "reduce" statement, the iterator that is reduced on still generates
   a "batch" of 2^16 random numbers before quantifying the gaussian pairs in
   the set.
 
   This 'elegant' version attempts to express the EP benchmark as succintly
   and readably as possible. This means that it does not perform quite as 
   well as the base Chapel implementation (about 2x slower), but because this
   implementation relies more on Chapel constructs such as "reduce", future
   implementations of the Chapel compiler and runtime could possibly make
   greater improvements in this version's performance.

   Written by Brandon Holt and Margaret Cahir, October 2011.
*/


use Random, Time, BlockDist, ReplicatedDist;

enum classVals {S, W, A, B, C, D};

const Class = {classVals.S..classVals.D};

const probSizes: [Class] int  = [ 24, 25, 28, 30, 32, 36 ];
const refSx:     [Class] real =				[-3.247834652034740e+03,
							 -2.863319731645753e+03,
							 -4.295875165629892e+03,
							  4.033815542441498e+04,
							  4.764367927995374e+04,
							  1.982481200946593e+05],

      refSy:     [Class] real =				[-6.958407078382297e+03,
							 -6.320053679109499e+03,
							 -1.580732573678431e+04,
							 -2.660669192809235e+04,
							 -8.084072988043731e+04,
							 -1.020596636361769e+05];
config const probClass    = classVals.S;
config const printTimings = false;

const m = probSizes[probClass];

param mk = 16,      //batch size
      nq = 10,      //number of concentric    
      nk = 2 ** mk; //number per batch

type RingsTuple = nq*real;

const mm = m - mk,  //log2(numBatches)
      nn = 2:int(64) ** mm; //number of batches

const epsilon = 1.0e-8,
      seed = 271828183:int(64);
	 
const WorkSpace = {0:int(64)..#nn},
      WorkDist  = WorkSpace dmapped Block(WorkSpace);

var totalTime: Timer;

writef(" NAS Parallel Benchmarks 2.4 -- EP Benchmark\n");
writef(" Number of random numbers generated:  %15i\n", 2**(m+1));
writef(" Number of locales:                   %15i\n", numLocales);
writef(" Number of tasks per locale:          %15i\n", dataParTasksPerLocale);

/* Call the random number generator functions and initialize
   the x-array to reduce the effects of paging on the timers.
   Also, call all mathematical functions that are used. Make
   sure these initializations cannot be eliminated as dead code. */

totalTime.start();

proc gaussPairsBatch(k: int(64), numPairs: int) {
	var x: [1..numPairs*2] real;
	var q: RingsTuple;
	var t1, t2, tx, ty, x1, x2: real;
	
	var startTime, randTime, gaussTime: real;

	var rs = new NPBRandomStream(real, seed=seed, parSafe=false);
	//Find starting seed for this batch
	rs.skipToNth(k * 2*nk);

	// Compute uniform pseudorandom numbers.
	for e in x do e = rs.getNext();
	
	/* Compute Gaussian deviates by acceptance-rejection method and 
	   tally counts in concentric square annuli.  This loop is not 
	   vectorizable. */
	var sx, sy = 0.0;
	for i in 1..numPairs {
		x1 = x[i*2-1]*2.0-1.0;
		x2 = x[i*2  ]*2.0-1.0;
		t1 = (x1)**2 + (x2)**2;
		if t1 < 1.0 {
			t2 = sqrt(-2.0 * log(t1) / t1);
			tx = x1 * t2;
			ty = x2 * t2;
			var l = max(abs(tx), abs(ty)):int;
			q[l] += 1;
			sx += tx;
			sy += ty;
		}
	}

	return (q, sx, sy);
}

//do each batch of work (in parallel) and accumulate results
var (total_q, total_sx, total_sy) =
		+ reduce [k in WorkDist] gaussPairsBatch(k, nk);

//calculate total gaussian count (should equal prob size)
var gc = + reduce [i in 0..#nq] total_q[i];

totalTime.stop();
var tm = totalTime.elapsed();

//verify
var verified = abs((total_sx - refSx[probClass])/total_sx) <= epsilon &&
	       abs((total_sy - refSy[probClass])/total_sy) <= epsilon;

var Mops = 2:int(64)**(m+1)/tm/1e6;

// Print Results
writeln();
writeln("EP Benchmark Results:");
if printTimings then writeln("CPU Time = ", tm);
writeln("N = 2^", m);
writeln("No. Gaussian Pairs = ", gc);
writef("Sums = %25.12er, %25.12er\n", total_sx, total_sy);
writeln("Counts:");
for i in 0..#nq do
  writef("%3i %13i\n", i+1, total_q[i]);
writeln();

//print_results()
writeln(" EP Benchmark Completed.");
writeln(" Class           =                        ", probClass);
writef(" Size            = %24i\n", 2:int(64)**(m+1));
writef(" Iterations      = %24i\n", 0);
if printTimings then 
  writef(" Time in seconds = %24r\n", tm);
writef(" Total tasks     = %24i\n", numLocales*dataParTasksPerLocale);
writef("    # Locales    = %24i\n", numLocales);
writef("    Tasks/Locale = %24i\n", dataParTasksPerLocale);
if printTimings {
  writef(" Mop/s total     = %24r\n", Mops);
  writef(" Mop/s/process   = %24r\n", Mops/numLocales);
}
writeln(" Operation type  = Random numbers generated");
writeln(" Verification    = ", if verified then
						   "              SUCCESSFUL" else
						   "                  FAILED");
