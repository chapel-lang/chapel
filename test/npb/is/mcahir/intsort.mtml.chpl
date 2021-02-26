/*
   NPB Integer Sort
   Chapel parallel version: multi-task, multi-locale
     based on bucket sort (same as NPB reference version)
  
   Originally written by Margaret Cahir (Cray, Inc.)
  
   NOTES on the Block version:
     Limitations:
       - works with 32-bit integers only
       - tested on 1, 2 and 4 locales, Classes S, A, B and C (not D)
  
   NOTES on the BlockCyclic version:
     Still need to
     - adjust to solve for Class D (means adjusting for int64)
     - can gen keys be made a little cleaner? (see note)
     - can a backwards index be made to work? see note in fullVerify
  	var mype: int = i / nkeys_per_pe;  //  can this be i >> log2nkeys_per_pe?
     - loop2: can we use scan in line below
  
   Modifications [sungeun@cray.com]: 12/2011
  
   - Combined Block and BlockCyclic versions
   
*/

// Problem size parameters
enum  classVals {S=0, W=1, A=2, B=3, C=4};
const Classes = {classVals.S..classVals.C};
const ClassNkeys:  [Classes] int = [ 16, 20, 23, 25, 27 ],
      ClassRanges: [Classes] int = [ 11, 16, 19, 21, 23 ];

// Set of config constants that control size of problem and output options
config const probClass = classVals.S;	 // default to Sample class
config const maxIterations = 10;	 // can be dialed down for devel runs
config const DEBUG:        bool = false; // shows progress and summary info
config param printLoopTimings: bool = false; // prints timing info
config param printArrays:  bool = false; // prints out arrays (can be long)
config const printTime    = false;	 // turn off for test verification
config const printNumLocales = true;     // print the number of locales

// Standard modules
use Time, Random, BlockDist, BlockCycDist;
enum ISDistType { block, blockcyclic };
config param distType = ISDistType.block;

// Comparison numbers for partial verification (these are given)
var testD: domain(2) =  {0..4, 0..4};	// # of Classes, # of keys to verify
var testIndexArray: [testD] int = (	( 48427,17148,23627,62548,4431),
					( 357773,934767,875723,898999,404505),
					( 2112377,662041,5336171,3642833,4250760),
					( 41869,812306,5102857,18232239,26860214),
					( 44172927,72999161,74326391,129606274,21736814) );
const testRankArray: [testD]  int = (   ( 0,18,346,64917,65463),
					( 1249,11698,1039987,1043896,1048018),
					( 104,17523,123928,8288932,8388264),
					( 33422937,10244,59149,33135281,99),
					( 61147,882988,266290,133997595,133525895) );

var passedVerifications = 0;		// verification counter

// Set problem sizes and storage requirements
const log2range = ClassRanges(probClass);
const log2nkeys = ClassNkeys (probClass);
var range:    int = 1 << log2range;	// left shift for power of 2 values
var nkeys:    int = 1 << log2nkeys;
var tasksPerLocale = if dataParTasksPerLocale == 0 
                     then here.maxTaskPar
                     else dataParTasksPerLocale;
var npes:     int = numLocales*tasksPerLocale;
var log2npes: int = 0;
while (1<<log2npes < npes) do {log2npes += 1; }
if 2**log2npes > npes {
  if DEBUG then writeln("Resizing npes to a power of 2");
  log2npes -= 1;
  npes = 2 ** log2npes;
  if npes % numLocales != 0 {
    writeln("Unable to resize number of tasks to a power of two.");
    writeln("The nearest power of two below npes is not divisible by numLocales");
    writeln("Provided number of tasks: ", numLocales * tasksPerLocale);
    halt();
  }
  tasksPerLocale = npes / numLocales;
}
var nbuckets: int = if distType==ISDistType.block then npes else 4*npes;

var keybuff_pe: real = 3*nkeys/npes;
var keybuffsz:  int  = keybuff_pe:int;

// Domains for main arrays
const keySpace:    domain(1)= {0..#nkeys};

// mcahir: Alas, it seems a block cyclic dist array has to be 2D so ended up making countSpace a 2D domain
const countSpace = if distType==ISDistType.block then {0..#range}
                                                 else {0..#range, 0..0};
const bucketSpace: domain(2)= {0..#npes,0..#nbuckets};
const buffSpace:   domain(2)= {0..#nbuckets,0..#keybuffsz};

// ... block a 2D space into horizontal slabs
const MyLocaleView = {0..#numLocales, 1..1};
const MyLocales: [MyLocaleView] locale = reshape(Locales, MyLocaleView);
const bucketDom = bucketSpace dmapped Block(boundingBox=bucketSpace, targetLocales=MyLocales, dataParTasksPerLocale=tasksPerLocale);
const buffDom = if distType==ISDistType.block
  then buffSpace dmapped Block(boundingBox=buffSpace, targetLocales=MyLocales, dataParTasksPerLocale=tasksPerLocale)
  else buffSpace dmapped BlockCyclic (startIdx=buffSpace.low,
                                      blocksize=(tasksPerLocale,
                                                 keybuffsz),
                                      targetLocales=MyLocales);

// Map domains to locales
const keyDom   = keySpace dmapped Block(boundingBox=keySpace, dataParTasksPerLocale=tasksPerLocale);
const countDom = if distType==ISDistType.block
  then countSpace dmapped Block(boundingBox=countSpace, dataParTasksPerLocale=tasksPerLocale)
  else countSpace dmapped BlockCyclic(startIdx=countSpace.low,
                                      blocksize=(tasksPerLocale,1),
                                      targetLocales=MyLocales);

// Now declare the main distributed arrays
var key:      [keyDom] int;	// random numbers between 0 and range -1
var rank:     [keyDom] int;	// store ranks here
var sortkey:  [keyDom] int;	// used for verification
var keybuff:  [buffDom]int;	// used for remote storage
var keycount: [countDom] int = 0;	// store #occurrences of values
var keycountbuf: [countDom] int = 0;	// buffer for pre-fix scan

// these counter arrays are also distributed
var bucket_cnts: [bucketDom] int;	// there is a row here for each pe (rank, locale)
var bucket_ptrs: [bucketDom] int;  // similarly, a row for each pe, a column for each bucket

// Timers
var tsetup, tsort, tverif: Timer;
var tloops: [1..5] Timer;

// Compute log of the number of pes and num keys per pe
var nkeys_per_pe: int = nkeys >> log2npes;
var log2nbuckets: int = 0;
while (1<<log2nbuckets < nbuckets) do {log2nbuckets += 1; }


proc main () {

  tsetup.start();

  writef ("NAS Parallel Benchmarks 2.4 -- IS Benchmark\n" );
  writef (" Size:                       %{#########}  (class %t)\n", nkeys, probClass);
  writef (" Iterations:                 %{#########}\n",maxIterations);
  if printNumLocales then
    writef (" Number of locales:          %{#########}\n",numLocales);
  writef (" Number of tasks per locale: %{#########}\n",dataParTasksPerLocale);
  writef (" \n");


  // initialize key values
  gen_keys();

  tsetup.stop();
  if (printTime) 
    {writeln ("Elapsed time for setup    =",tsetup.elapsed()," seconds");}
  
  if (DEBUG) then writeln("npes =",npes," log2npes=",log2npes);
  // ," log2nbuckets=", if ISDistType==block then "N/A" else log2nbuckets);

  tsort.start();				// start the timed section
						// main iteration loop
  for iteration in 1..maxIterations {
  
    if (probClass != classVals.S) then writeln (iteration," Iteration ");
  
    key[iteration] = iteration;
    key[iteration+maxIterations] = range - iteration;
  
    rank_keys ( iteration );
  
    partialVerification(iteration, keycountbuf);
  
  }						// end main iteration loop

  tsort.stop();					// end the timed section
  if (printTime) {
    writeln ("Elapsed time for key sort =", tsort.elapsed()," seconds");}
  if (printTime) {
    writeln ("Time for individual loops");
    for i in 1..5 do writeln("  Loop ",i,"  ",tloops(i).elapsed()," seconds"); }

  tverif.start();  				// full verification (full sort)
  keycount = keycountbuf;
  fullVerify();
  tverif.stop();
  if (printTime) 
    {writeln ("Elapsed time for full verify =",tverif.elapsed()," seconds");}


  // Now complete output

  writef("\n\n IS Benchmark Completed\n");
  writef(" Class           =                         %t\n",probClass);
  writef(" Size            = %{#########################}\n",nkeys);
  writef(" Iterations      = %{#########################}\n",maxIterations);
  if (printTime) {
      writef(" Time in seconds = %{######################.##}\n",
        tsort.elapsed(TimeUnits.seconds));
      writef(" Mop/s total     = %{######################.##}\n",
        (maxIterations*nkeys)/tsort.elapsed(TimeUnits.seconds)/1000000 );
  }
  writeln(" Operation type  = ","              keys ranked");
  if (passedVerifications ==  (maxIterations*5+1) ) then
    writeln(" Verification    =                SUCCESSFUL");
  else
    writeln(" Verification    =              UNSUCCESSFUL");


} // end of main program


proc rank_keys ( iteration ) {

  bucket_cnts = 0;			// initialize bucket_cnts and bucket_ptrs
  bucket_ptrs = 0;			//   for each iteration
  keycount    = 0;

  // count the number of occurrences of each key value (each pe can do their share independently)

  tloops(1).start();
  forall (k,i) in zip(key, keyDom) do {
    on k do {
      var ibucket: int = k >> (log2range-log2nbuckets);  // this is equivalent to (key(i)/range)*nbuckets
      var mype: int = i / nkeys_per_pe;
      if false {
        writeln("mype=",mype," i=",i," key=",k," ibucket=",ibucket);
      }
      bucket_cnts[mype, ibucket] += 1;
    }
  }

  tloops(1).stop();
  if (printLoopTimings) then writeln("time for first loop =",tloops(1).elapsed());
  if (DEBUG) {
    var totbucket: [0..nbuckets-1] int;
    for ibucket in 0..nbuckets-1 {
      totbucket[ibucket] = + reduce (bucket_cnts[0..npes-1,ibucket]);
    }
    writeln ("total # keys by bucket= ",totbucket);
  }
  if (printArrays) { writeln("bucket_cnts"); writeln (bucket_cnts); }

  // need to synchronize here - co/forall construct above takes care of that for us

  // figure out pointers for keybuff -- even though these are remote references, 
  // each pe can pre-fix scan a column independently

  var hi0 = if distType==ISDistType.block then 0..npes-1
    else 0..nbuckets-1;
  var hi1 = if distType==ISDistType.block then 1..nbuckets-1
    else 1..npes-1;
  tloops(2).start();
  forall b in hi0 do {
    for i in hi1 {
      bucket_ptrs[i, b] = bucket_ptrs[i-1, b] + bucket_cnts[i-1, b];
      if false then writeln("bucket_ptrs", (i,b), "=", bucket_ptrs[i, b]);
    }
  }

  tloops(2).stop();
  if (printLoopTimings) then writeln("time for second loop =",tloops(2).elapsed());
  if (printArrays) { writeln ("bucket_ptrs"); writeln (bucket_ptrs); }

  // now send off the keys -- there are remote references here, but there are no race conditions,
  //                          that is, each pe can work independently
  tloops(3).start();
  forall (k,i) in zip(key, keyDom) do {
    on k do {
      var mype: int = i / nkeys_per_pe;   // change this to a shift?
      var ibucket: int;
      ibucket = k >> (log2range-log2nbuckets);
      if false {writeln("2 mype=",mype," i=",i," key=",k," ibucket=",ibucket);}
      if false then writeln("2 bucket_ptrs", (mype,ibucket), "=", bucket_ptrs[mype,ibucket]);
      keybuff(ibucket, bucket_ptrs[mype,ibucket]) = k;
      bucket_ptrs[mype, ibucket] += 1;
    }
  }
  tloops(3).stop();
  if (printLoopTimings) then writeln("time for 3rd loop =",tloops(3).elapsed());
  if (printArrays) { writeln ("keybuff");     writeln (keybuff); }
  if (printArrays) { writeln ("bucket_ptrs"); writeln (bucket_ptrs); }

  // need to sychronize here - implicit sync of coforall takes care of it for us

  // now think of operating in an alternate domain
  // each pe does this bit
  tloops(4).start();
  forall b in 0..nbuckets-1 do {
    on keybuff[b,0] do     {			// is this necessary?
      var mykeys: int = bucket_ptrs[npes-1, b]; 
      for i in 0..mykeys-1 {
        var bindx = if distType==ISDistType.block then keybuff[b,i]
          else (keybuff[b,i], 0);
        keycount[bindx] += 1;
      }
    }
  }
  tloops(4).stop();
  if (printLoopTimings) then writeln("time for 4th loop =",tloops(4).elapsed());
  if (printArrays) {writeln ("keycount"); writeln (keycount);}

  // prefix scan the keycount array - this is a global operation, everyone participates
  tloops(5).start();
  keycountbuf = + scan (keycount);
  /*
  if distType==ISDistType.block {
    for i in 1..range-1 {
      keycount[i] += keycount[i-1];
    }
    keycountbuf = keycount;
  } else {
  }
  // keycountbuf = + scan (keycount);
  */
  tloops(5).stop();
  if printLoopTimings then writeln("time for 5th loop =",tloops(5).elapsed());
  if printArrays { writeln ("scanned keycountbuf"); writeln (keycountbuf); }

}


proc partialVerification(iteration: int, keyBuff1 /*:[countDom]int*/ ) {

  //  Performs a partial verification after each iteration.
  //  This code is pretty much a straight translation from the C reference 
  //  version except that we use enums to access the comparison values directly.

  for i in 0..4 {
    var k = key(testIndexArray(probClass:int,i));
    var kbidx = if distType==ISDistType.block then k-1 else (k-1,0);
    select probClass {
      when classVals.S do {
        if (i <= 2) {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i)+iteration) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i)-iteration) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
      when classVals.W do {
        if (i < 2) {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i) + (iteration-2)) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i) - iteration) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
      when classVals.A do {
        if (i <= 2) {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i) + (iteration-1)) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i) - (iteration-1)) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
      when classVals.B do {
        if (i == 1 || i == 2 || i == 4) {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i) + iteration) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i) - iteration) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
      when classVals.C do {
        if (i <= 2) {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i) + iteration) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (keyBuff1(kbidx) != testRankArray(probClass:int,i) - iteration) {
            writeln("Failed partial verification: iteration ",
                    iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
    }
  }
  if (DEBUG) {writeln("After partial verification, passedVerifications=",passedVerifications); }
}	// end partial verification

proc fullVerify () {

  // Perform a full verification after all iterations are complete.
  // Since the keys were never actually sorted, we need to do that first. 

  for mype in 0..nbuckets-1 do {
    var mykeys: int = bucket_ptrs[npes-1, mype]; 
    var val: int;  
    for i in 0..mykeys-1 {
      var val = if distType==ISDistType.block then keybuff[mype,i]
        else (keybuff[mype,i],0);
      keycount[val] -= 1;
      keybuff[mype,i] = keycount[val];  // keycount is now the row, reuse keybuff for local storage
    }
  }
  if (printArrays) {writeln ("keycount"); writeln (keycount);}
  if (printArrays) {writeln ("keybuff "); writeln (keybuff);}
  
  // store the ranks in the rank array -- think of being back on the orginal domain
  // first, adjust the bucketptr array
  
  forall mype in 0..npes-1 do {
    var first: int = mype*nkeys_per_pe;
    var last:  int = first+nkeys_per_pe-1;
    var ibucket: int;
    // var r = last..first by -1;
    var i: int = last;
    for ii in first..last {			// going backwards
      ibucket = key(i) >> (log2range-log2nbuckets);
      bucket_ptrs[mype,ibucket] -= 1;
      if (DEBUG) {writeln(" indx=",i," key=",key(i)," ibucket=",ibucket,
                          " bucket_ptr=", bucket_ptrs[mype,ibucket], 
                          " keybuff =",  keybuff[ibucket, bucket_ptrs[mype,ibucket]]); }
      rank[i] = keybuff[ibucket, bucket_ptrs[mype,ibucket]];
      i = i -1;
    }                         // end for
  }                           // end coforall

  // Finally, sort the keys
  for i in 0..nkeys-1 { sortkey[rank[i]] = key[i]; }

  if (printArrays) {writeln ("Rank "); writeln (rank);}
  if (printArrays) {writeln ("Sortd="); writeln(sortkey);}

  // Verify that the keys are sorted correctly
  if ( && reduce ( sortkey[0..nkeys-2] <= sortkey[1..nkeys-1] ) ) then {passedVerifications += 1;}
  if (DEBUG) {writeln ("full Verify: passedVerifications=",passedVerifications);}

} 

proc gen_keys () {
  // initialize key values - do this on each rank or locale such that the key values 
  // are the same no matter how many locales or ranks are used
  // -- come back later and see if this can be simplified w/ a local range
 
  coforall loc in Locales do {
    on loc do {
      var tmpreals: [1..4] real;
      var seed: int(64) = 314159265;
 
      var mype:  int = here.id;
      var first: int = mype*(nkeys/numLocales);
      var last : int = first+(nkeys/numLocales) - 1;
 
      var rs = new owned NPBRandomStream(real, seed);
      rs.skipToNth(first*4);
      for i in first..last {
        rs.fillRandom(tmpreals);
        key(i) = ( (range>>2)*(+ reduce tmpreals ) ): int;
      }
    }
  }
}
