/*
 * The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org
 *
 * Original C contributed by Sebastien Loisel
 * Conversion to Chapel by Albert Sidelnik
 */

use BlockDist, IO;
config const n = 500 : int(64);


/* Return: 1.0 / (i + j) * (i + j +1) / 2 + i + 1; */
proc eval_A(i,j) : real
{
  /*
   * 1.0 / (i + j) * (i + j +1) / 2 + i + 1;
   * n * (n+1) is even number. Therefore, just (>> 1) for (/2)
   */
  var d = (((i + j) * (i + j + 1)) >> 1) + i + 1;
  return 1.0 / d;
}

proc eval_A_times_u(U : [] real, inRange, Au : [] real)
{
  forall (au,i) in zip(Au, {0..#inRange}) do 
    au = + reduce [j in 0..#inRange] (U(j) * eval_A(i,j));

}

proc eval_At_times_u(U : [] real, inRange, Au : [] real)
{
  forall (au,i) in zip(Au, {0..#inRange}) do
    au = + reduce [j in 0..#inRange] (U(j) * eval_A(j,i));
}

proc eval_AtA_times_u(u,AtAu,v : [] real, inRange)
{
     eval_A_times_u(u, inRange, v);
     eval_At_times_u(v, inRange, AtAu);
}

proc main() {
  var Dist = new dmap(new Block(rank=1, idxType=int(64), boundingBox={0..#n},
                                      dataParTasksPerLocale=here.maxTaskPar));
  var Dom : domain(1, int(64)) dmapped Dist = {0..#n};

  var tmp, U, V : [Dom] real;

  U = 1.0;

  for 1..10 do {
    eval_AtA_times_u(U, V, tmp, n);
    eval_AtA_times_u(V, U, tmp, n);
  }

  var vv = + reduce [v in V] (v * v);
  var vBv = + reduce [(u,v) in zip(U,V)] (u * v);

  const res = sqrt(vBv/vv);
  writeln(res, new iostyle(precision=10));
}
