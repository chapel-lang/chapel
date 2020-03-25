/*
 * The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org
 *
 * Original C contributed by Sebastien Loisel
 * Conversion to Chapel by Albert Sidelnik
 */
use IO;

config const n = 500;

use barrierWF;
var b: owned BarrierWF?;

/* Return: 1.0 / (i + j) * (i + j +1) / 2 + i + 1; */
proc eval_A(i,j : int) : real
{
  /*
   * 1.0 / (i + j) * (i + j +1) / 2 + i + 1;
   * n * (n+1) is even number. Therefore, just (>> 1) for (/2)
   */
  var d : int = (((i + j) * (i + j + 1)) >> 1) + i + 1;
  return 1.0 / d;
}

proc eval_A_times_u(u : [] real, inRange : int, Au : [] real, outRange1, outRange2 : int)
{
  for i in outRange1..outRange2-1 do {
    Au(i) = + reduce [j in 0..inRange-1] (u(j) * eval_A(i,j));
  }
}

proc eval_At_times_u(u : [] real, inRange : int, Au : [] real, outRange1, outRange2 : int)
{
  for i in outRange1..outRange2-1 do
    Au(i) = + reduce [j in 0..inRange-1] (u(j) * eval_A(j,i));
}

proc eval_AtA_times_u(u,AtAu,v : [] real, inRange, range1, range2 : int)
{
           eval_A_times_u(u, inRange, v, range1, range2);
           b!.barrier();
           eval_At_times_u(v, inRange, AtAu, range1, range2);
           b!.barrier();
}

proc main() {
  var tmp, u, v : [0..#n] real;
  var numThreads = here.maxTaskPar;
  var chunk = n / numThreads;

  u = 1.0;
  b = new owned BarrierWF(numThreads);

  coforall i in 0..#numThreads do {
    var r_begin = i * chunk;
    var r_end : int;
    if (i < (numThreads - 1)) then
      r_end = r_begin + chunk;
    else
      r_end = n;

    for 1..10 do {
      eval_AtA_times_u(u, v, tmp, n, r_begin, r_end);
      eval_AtA_times_u(v, u, tmp, n, r_begin, r_end);
    }
  }

  var   vv  = + reduce (v * v);
  var   vBv = + reduce (u * v);
  const res = sqrt(vBv/vv);

  writeln(res, new iostyle(precision=10));
}
