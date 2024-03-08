// A Chapel-tastic version of gcc-loops
// See http://gcc.gnu.org/projects/tree-ssa/vectorization.html
// This is not 1-1 translation of the code, efforts were made to take advantage
// of Chapel features and expand the tests

use Time;
import Math;

config param correctness = false;
config param printTiming = false;
config param perfTesting = false;

config param warmupIterations = 100;
config param measureIterations = 50;

config param N = 256;
config type elmType = int(32);

record aRecord {
  const D: domain(1);
  proc init(D) do this.D = D;

  var A: [0..#N] elmType;
  proc ref this(i) ref {
    return A[i];
  }
  iter ref these() ref {
    foreach a in A do yield a;
  }
}
class aClass {
  const D: domain(1);
  proc init(D) do this.D = D;

  var A: [0..#N] elmType;
  proc this(i) ref {
    return A[i];
  }
  iter these() ref {
    foreach a in A do yield a;
  }
}


// must be global to be captured by FCP
const D = 0..#N;
var A, B, C, E: [D] elmType;
var myRecord = new aRecord(D);
var myClass = new aClass(D);

// mostly a config to prevent the loop being optimized away completely
config const x: elmType = 17;

var retVal: elmType;

proc printVals(indent="") {
  writeln(indent, "A=", A);
  writeln(indent, "B=", B);
  writeln(indent, "C=", C);
  writeln(indent, "myRecord=", myRecord);
  writeln(indent, "myClass=", myClass);
  writeln(indent, "retVal=", retVal);
}

proc test1(ref A: [?D] elmType, B: [D] elmType, C: [D] elmType) {
  @llvm.assertVectorized
  foreach i in D {
    A[i] = B[i] + C[i];
  }
}

proc test2(ref A: [?D] elmType, B: [D] elmType, C: [D] elmType) {
  @llvm.assertVectorized
  foreach (a,b,c) in zip(A,B,C)  {
    a = b + c;
  }
}

proc test3(ref A: [?D] elmType, x: elmType) {
  @llvm.assertVectorized
  foreach i in D {
    A[i] = x;
  }
}

proc test4(ref A: [?D] elmType, x: elmType) {
  @llvm.assertVectorized
  foreach a in A {
    a = x;
  }
}

proc test5(ref A: [?D] elmType, B: [D] elmType, x: elmType) {
  @llvm.assertVectorized
  foreach i in D {
    A[i] = B[i] + x;
  }
}

proc test6(ref A: [?D] elmType, B: [D] elmType, x: elmType) {
  @llvm.assertVectorized
  foreach (a,b) in zip(A,B) {
    a = b + x;
  }
}

proc test7(ref A: [?D] elmType, B: [D] elmType, C: [D] elmType) {
  @llvm.assertVectorized
  foreach i in D {
    A[i] = B[i]:int(numBits(elmType)) & C[i]:int(numBits(elmType)); // cast supports reals
  }
}

proc test8(ref A: [?D] elmType, B: [D] elmType, C: [D] elmType) {
  @llvm.assertVectorized
  foreach (a,b,c) in zip(A,B,C)  {
    a = b:int(numBits(elmType)) & c:int(numBits(elmType)); // cast supports reals
  }
}

proc test9(ref A: [?D] elmType, B: [D] elmType, C: [D] elmType) where D.rank == 1 {
  @llvm.assertVectorized
  foreach i in D.low..D.high/2 {
    A[i] = B[i+1] + C[i+3];
  }
}
proc test9(ref A: [?D] elmType, B: [D] elmType, C: [D] elmType) where D.rank == 2 {
  const localD = {D(0).low..D(0).high/2, D(1).low..D(1).high/2};
  @llvm.assertVectorized
  foreach (i,j) in localD {
    A[i,j] = B[i+1,j+1] + C[i+3,j+3];
  }
}

proc test10(ref A: [?D] elmType, B: [D] elmType, MAX: elmType) {
  @llvm.assertVectorized
  foreach i in D {
    var x = B[i];
    A[i] = if x > MAX then MAX else 0:elmType;
  }
}

proc test11(ref A: [?D] elmType, B: [D] elmType, MAX: elmType) {
  @llvm.assertVectorized
  foreach (a,b) in zip(A,B) {
    a = if b > MAX then MAX else 0:elmType;
  }
}

proc test12(ref r: aRecord, x: elmType) {
  @llvm.assertVectorized
  foreach i in r.A.domain {
    r.A[i] = x;
  }
}

proc test13(ref r: aRecord, x: elmType) {
  @llvm.assertVectorized
  foreach i in r.A.domain {
    r[i] = x;
  }
}

proc test14(ref r: aRecord, x: elmType) {
  @llvm.assertVectorized
  foreach a in r.A {
    a = x;
  }
}

proc test15(ref r: aRecord, x: elmType) {
  @llvm.assertVectorized
  foreach a in r {
    a = x;
  }
}

proc test16(ref r: aClass, x: elmType) {
  @llvm.assertVectorized
  foreach i in r.A.domain {
    r.A[i] = x;
  }
}

proc test17(ref r: aClass, x: elmType) {
  @llvm.assertVectorized
  foreach i in r.A.domain {
    r[i] = x;
  }
}

proc test18(ref r: aClass, x: elmType) {
  @llvm.assertVectorized
  foreach a in r.A {
    a = x;
  }
}

proc test19(ref r: aClass, x: elmType) {
  @llvm.assertVectorized
  foreach a in r {
    a = x;
  }
}

proc test20(ref A: [?D] elmType, B: [D] elmType, C: [D] elmType) {
  @llvm.assertVectorized
  foreach i in D {
    A[i] = Math.log(B[i]):elmType + Math.sqrt(C[i]):elmType;
  }
}

proc test21(A: [?D] elmType, B: [D] elmType, ref diff: elmType) {
  @llvm.assertVectorized
  for i in D {
    diff += B[i] - A[i];
  }
}

proc test22(A: [?D] elmType, B: [D] elmType, ref diff: elmType) {
  @llvm.assertVectorized
  for (a,b) in zip(A,B)  {
    diff += b - a;
  }
}

proc test23(ref A: [?D] elmType, B: [D] elmType, C: [D] elmType, ref E: [D] elmType) {
  @llvm.assertVectorized
  foreach i in D.low..D.high/2 {
    A[i] = B[2*i+1] * C[2*i+1] - B[2*i] * C[2*i];
    E[i] = B[2*i] * C[2*i+1] + B[2*i+1] * C[2*i];
  }
}

proc test24(ref A: [?D] elmType) {
  @llvm.assertVectorized
  for i in D  {
    A[i] = i:elmType;
  }
}

proc test25(ref A: [?D] elmType) {
  @llvm.assertVectorized
  foreach (a,i) in zip(A,D)  {
    a = i:elmType;
  }
}


//   for (i = 0; i < M; i++) {
//     diff = 0;
//     for (j = 0; j < N; j+=8) {
//       diff += (a[i][j] - b[i][j]);
//     }
//     out[i] = diff;
//   }

//   for (k = 0; k < K; k++) {
//     sum = 0;
//     for (j = 0; j < M; j++)
//       for (i = 0; i < N; i++)
//           sum += in[i+k][j] * coeff[i][j];

//     out[k] = sum;
//   }



//   for (j = 0; j < M; j++)
//     {
//       x = x_in[j];
//       curr_a = a[0];

//       for (i = 0; i < N; i++)
//         {
//           next_a = a[i+1];
//           curr_a = x > c[i] ? curr_a : next_a;
//         }

//       x_out[j] = curr_a;
//     }


//   for (i = 0; i < N; i++)
//     {
//        a = *pInput++;
//        b = *pInput++;
//        c = *pInput++;

//        *pOutput++ = M00 * a + M01 * b + M02 * c;
//        *pOutput++ = M10 * a + M11 * b + M12 * c;
//        *pOutput++ = M20 * a + M21 * b + M22 * c;
//     }

// int sum1;
// int sum2;
// int a[128];
// void foo (void)
// {
//   int i;

//   for (i = 0; i < 64; i++)
//     {
//       sum1 += a[2*i];
//       sum2 += a[2*i+1];
//     }
// }


// int sum;
// int a[128];
// void foo (void)
// {
//   int i;

//   for (i = 0; i < 64; i++)
//     {
//       sum += a[2*i];
//       sum += a[2*i+1];
//     }
// }




// void foo (int * __restrict__ dst, short * __restrict__ src,
//           int h, int stride, short A, short B)
// {
//   int i;
//   for (i = 0; i < h; i++)
//     {
//       dst[0] += A*src[0] + B*src[1];
//       dst[1] += A*src[1] + B*src[2];
//       dst[2] += A*src[2] + B*src[3];
//       dst[3] += A*src[3] + B*src[4];
//       dst[4] += A*src[4] + B*src[5];
//       dst[5] += A*src[5] + B*src[6];
//       dst[6] += A*src[6] + B*src[7];
//       dst[7] += A*src[7] + B*src[8];
//       dst += stride;
//       src += stride;
//     }
// }


// int foo (int *b, int n)
// {
//   int i, a = 0;

//   for (i = n-1; i ≥ 0; i--)
//     a += b[i];

//   return a;
// }


// void foo (unsigned short *src, unsigned int *dst)
// {
//   int i;

//   for (i = 0; i < 256; i++)
//     *dst++ = *src++ << 7;
// }



// #define N 1024
// float a[N], b[N];
// int c[N];

// void foo (short x, short y)
// {
//   int i;
//   for (i = 0; i < N; i++)
//     c[i] = a[i] < b[i] ? x : y;
// }



// #define N 1024
// float a[N], b[N], c[N], d[N];
// int j[N];

// void foo (void)
// {
//   int i;
//   _Bool x, y;
//   for (i = 0; i < N; i++)
//     {
//       x = (a[i] < b[i]);
//       y = (c[i] < d[i]);
//       j[i] = x & y;
//     }
// }




var state: uint(8) = 1;
proc initArray(ref arr: [?d]) {
  for i in d {
    state *= 7; state ^= 0x27; state += 1;
    arr[i] = state;
  }
}

var testnum = 1;
proc bench(func, warmup: int, iterations: int) do bench(func, none, warmup, iterations);
proc bench(func, args, warmup: int, iterations: int) {

  for 0..#warmup {
    if args.type != nothing
      then func((...args));
      else func();
  }

  var timer = new stopwatch();
  timer.start();
  for 0..#iterations {
    if args.type != nothing
      then func((...args));
      else func();
  }
  var t = timer.elapsed();

  if printTiming {
    writeln("Test #", testnum, " Timing");
    writeln("  Total: ", t);
    writeln("    Avg: ", t/iterations);
  }

  if perfTesting {
    writeln("Test-", testnum, "-", elmType:string, ": ", t/iterations, " seconds");
  }

  if correctness {
    writeln("Test #", testnum, "Correctness");
    writeln("  Before: ");
    printVals(indent="    ");
    if args.type != nothing
      then func((...args));
      else func();
    writeln("  After: ");
    printVals(indent="    ");
  }

  testnum += 1;
}

proc main() {

  initArray(A);
  initArray(B);
  initArray(C);
  initArray(myRecord.A);
  initArray(myClass.A);

  inline proc test1_shim() do test1(A,B,C);
  bench(test1_shim, warmupIterations, measureIterations);

  inline proc test2_shim() do test2(A,B,C);
  bench(test2_shim, warmupIterations, measureIterations);

  inline proc test3_shim() do test3(A,x);
  bench(test3_shim, warmupIterations, measureIterations);

  inline proc test4_shim() do test4(A,x);
  bench(test4_shim, warmupIterations, measureIterations);

  inline proc test5_shim() do test5(A,B,x);
  bench(test5_shim, warmupIterations, measureIterations);

  inline proc test6_shim() do test6(A,B,x);
  bench(test6_shim, warmupIterations, measureIterations);

  inline proc test7_shim() do test7(A,B,C);
  bench(test7_shim, warmupIterations, measureIterations);

  inline proc test8_shim() do test8(A,B,C);
  bench(test8_shim, warmupIterations, measureIterations);

  inline proc test9_shim() do test9(A,B,C);
  bench(test9_shim, warmupIterations, measureIterations);

  inline proc test10_shim() do test10(A,B,x);
  bench(test10_shim, warmupIterations, measureIterations);

  inline proc test11_shim() do test11(A,B,x);
  bench(test11_shim, warmupIterations, measureIterations);

  inline proc test12_shim() do test12(myRecord,x);
  bench(test12_shim, warmupIterations, measureIterations);

  inline proc test13_shim() do test13(myRecord,x);
  bench(test13_shim, warmupIterations, measureIterations);

  inline proc test14_shim() do test14(myRecord,x);
  bench(test14_shim, warmupIterations, measureIterations);

  inline proc test15_shim() do test15(myRecord,x);
  bench(test15_shim, warmupIterations, measureIterations);

  inline proc test16_shim() do test16(myClass,x);
  bench(test16_shim, warmupIterations, measureIterations);

  inline proc test17_shim() do test17(myClass,x);
  bench(test17_shim, warmupIterations, measureIterations);

  inline proc test18_shim() do test18(myClass,x);
  bench(test18_shim, warmupIterations, measureIterations);

  inline proc test19_shim() do test19(myClass,x);
  bench(test19_shim, warmupIterations, measureIterations);

  inline proc test20_shim() do test20(A,B,C);
  bench(test20_shim, warmupIterations, measureIterations);

  inline proc test21_shim() do test21(A,B,retVal);
  bench(test21_shim, warmupIterations, measureIterations);

  inline proc test22_shim() do test22(A,B,retVal);
  bench(test22_shim, warmupIterations, measureIterations);

  inline proc test23_shim() do test23(A,B,C,E);
  bench(test23_shim, warmupIterations, measureIterations);

  inline proc test24_shim() do test24(A);
  bench(test24_shim, warmupIterations, measureIterations);

  inline proc test25_shim() do test25(A);
  bench(test25_shim, warmupIterations, measureIterations);
}
