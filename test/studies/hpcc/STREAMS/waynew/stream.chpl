// HPCC STREAM benchmark.  Basis was stream.c

param TOTAL_MBS = 3;          // total memory
param MB = 1024*1024;         // megabytes
param GB = 1024*MB;
param MEMORY = TOTAL_MBS*MB;
param NUM_VECTORS = 3;        
param OFFSET = 0;             // purpose?
param SCALAR = 3.0;

// parameters of execution (from include/hpcc.h); bunch of stuff deleted
record HPCC_Params {
  // var StreamThreads, StreamVectorSize: int ;  // should these be used?
  // var HPLMaxProc, HPLMinProc: int;            // should these be used?
  var HPLMaxProcMem: int;                        // totalMem/procMin
}


// from src/io.c
proc
HPCC_LocalVectorSize( params: HPCC_Params, 
                      vecCnt: int, 
                      size: int, 
                      pow2: bool): int {
  // use our bit lib routine instead ?

  // this is the maximum power of 2 that that can be held in a signed integer 
  // (for a 4-byte integer, 2**31-1 is the maximum integer, so the maximum 
  // power of 2 is 30) */

  var maxIntBits2 = numBits(int) - 2;

  // flg2 = floor(log2(params->HPLMaxProcMem / size / vecCnt))
  var flg2 = 1;
  while (params.HPLMaxProcMem/size)/vecCnt >> flg2 do {
    flg2 += 1;
  }
  flg2 -= 1;

  if flg2 <= maxIntBits2 {
    if pow2 then return 1 << flg2;
    return (params.HPLMaxProcMem/size)/vecCnt;
  }

  return 1 << maxIntBits2;
}


// stream.c

var VectorSize: int;
var Vector: domain(1);
var a, b, c: [Vector] real;

// param N = 2000000;      // purpose?
param NTIMES = 10;

var HLINE = "-------------------------------------------------------------";


// return failure
proc checkSTREAMresults( doIO: bool): bool {
  // reproduce initialization
  var aj = 1.0;
  var bj = 2.0;
  var cj = 0.0;

  // a[] is modified during timing check
  aj = 2.0E0 * aj;

  // now execute timing loop
  var scalar = SCALAR;
  for k in 1..NTIMES {
    cj = aj;
    bj = scalar * cj;
    cj = aj + bj;
    aj = bj + scalar * cj;
  }
  aj = aj * VectorSize;
  bj = bj * VectorSize;
  cj = cj * VectorSize;

  var asum = 0.0;
  var bsum = 0.0;
  var csum = 0.0;
  for j in Vector {
    asum += a[j];
    bsum += b[j];
    csum += c[j];
  }

  if doIO {
    writeln( "Results Comparison: ");
    writeln( "        Expected  : ", aj, bj, cj);
    writeln( "        Observed  : ", asum, bsum, csum);
  }

  // var epsilon = 1.e-8;
  var epsilon = 0.00000001;
  var failure = true;
  if (abs(aj-asum)/asum > epsilon) {
    if doIO {
      writeln( "Failed Validation on array a[]");
      writeln( "        Expected  : ", aj);
      writeln( "        Observed  : ", asum);
    }
  } else if (abs(bj-bsum)/bsum > epsilon) {
    if doIO {
      writeln( "Failed Validation on array b[]");
      writeln( "        Expected  : ", bj);
      writeln( "        Observed  : ", bsum);
    }
  } else if (abs(cj-csum)/csum > epsilon) {
    if doIO {
      writeln( "Failed Validation on array c[]");
      writeln( "        Expected  : ", cj);
      writeln( "        Observed  : ", csum);
    }
  } else {
    failure = false;
    if doIO then writeln( "Solution Validates");
  }
  return failure;
}


proc HPCC_Stream( params: HPCC_Params,  doIO: bool): bool {
  VectorSize = HPCC_LocalVectorSize( params, NUM_VECTORS, numBits(real)/8, false); 

  Vector = {1..VectorSize};   // realloc a, b, and c

  // How do we handle allocation errors?

  // --- SETUP --- determine precision and check timing ---
  if doIO {
    writeln( HLINE);
    var BytesPerWord = numBits(real)/8;
    writeln( "This system uses ", BytesPerWord, " bytes per DOUBLE PRECISION word.");

    writeln( HLINE);
    writeln( "Array size (B) = ", VectorSize*BytesPerWord, " Offset = " , OFFSET);
    writeln( "Total memory required (GB) = ", (NUM_VECTORS * BytesPerWord) * (VectorSize / GB:real));
  }


  // Get initial value for system clock.
  [j in Vector] {
    a[j] = 1.0;
    b[j] = 2.0;
    c[j] = 0.0;
  }
  // Alternatively?  Less opportunity to optimize?
  // a = 1.0;
  // b = 2.0;
  // c = 0.0;

  if doIO then writeln( HLINE);

  for j in Vector do 
    a[j] = 2.0E0 * a[j];

  // --- MAIN LOOP --- repeat test cases NTIMES times ---
  var scalar = SCALAR;
  for k in 1..NTIMES {
    // tuned_STREAM_Copy();
    // [j in Vector] c[j] = a[j];
    c = a;

    // tuned_STREAM_Scale(scalar);
    // [j in Vector] b[j] = scalar * c[j];
    b = scalar * c;

    // tuned_STREAM_Add();
    // [j in Vector] c[j] = a[j] + b[j];
    c = a + b;

    // tuned_STREAM_Triad(scalar);
    // [j in Vector] a[j] = b[j] + scalar*c[j];
    a = b + scalar * c;
  }

  // --- SUMMARY ---
  // timing output...

  // --- Check Results ---
  var failure = checkSTREAMresults( doIO);

  // free a, b, and c?

  return failure;
}


// main
var params: HPCC_Params;
params.HPLMaxProcMem = MEMORY;


if HPCC_Stream( params, true) {  // if failure
  writeln( "Solution Failed!");
}
