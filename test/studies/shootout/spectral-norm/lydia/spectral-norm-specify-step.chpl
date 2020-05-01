/*
 * The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org
 *
 * Original C contributed by Sebastien Loisel
 * Conversion to Chapel by Albert Sidelnik
 * Updated by Lydia Duncan
 */
use IO;

config const n = 500 : int(64);
config const step = 2 : int(64);
// step allows for control over the reduction, i.e. how parallel/serial it is

/* Return: 1.0 / (i + j) * (i + j +1) / 2 + i + 1; */
inline proc eval_A(i, j) : real
{
  /*
   * 1.0 / (i + j) * (i + j +1) / 2 + i + 1;
   * n * (n+1) is even number. Therefore, just (>> 1) for (/2)
   */
  const d = (((i + j) * (i + j + 1)) >> 1) + i + 1;
  return 1.0 / d;
}

proc sum (i, j, inRange, flip : bool, U : [] real) {
  var sum : real;
  var end = j + step - 1;
  if (end <= inRange - 1) { 
    for k in j..j+step-1 {
      var eval: real;
      if (!flip) then
        eval = eval_A(i, k);
      else
        eval = eval_A(k, i);
      sum += eval * U(k);
    }
  }
  return sum;
}

proc finishUp (i, inRange, U, Au, flip: bool) {
  if ((inRange % step) != 0) then {
    for j in (inRange - (inRange % step))..inRange-1 {
      var eval: real;
      if (!flip) then
        eval = eval_A(i, j);
      else
        eval = eval_A(j, i);
      Au(i) += eval * U(j);
    }
  }
}

proc eval_A_times_u(U : [] real, inRange, Au : [] real)
{
  forall i in 0..#inRange { 
    Au(i) = + reduce [j in 0..#inRange by step] (sum(i, j, inRange, false, U));
    finishUp(i, inRange, U, Au, false);
  }
}

proc eval_At_times_u(U : [] real, inRange, Au : [] real)
{
  forall i in 0..#inRange {
    Au(i) = + reduce [j in 0..#inRange by step] (sum(i, j, inRange, true, U));
    finishUp(i, inRange, U, Au, true);
  }
}

proc eval_AtA_times_u(u, AtAu, v : [] real, inRange)
{
     eval_A_times_u(u, inRange, v);
     eval_At_times_u(v, inRange, AtAu);
}

proc main() {
  var tmp, U, V : [0..#n] real;

  U = 1.0;

  for 1..10 do {
    eval_AtA_times_u(U, V, tmp, n);
    eval_AtA_times_u(V, U, tmp, n);
  }

  const vv = + reduce [v in V] (v * v);
  const vBv = + reduce [(u,v) in zip(U,V)] (u * v);

  const res = sqrt(vBv/vv);
  writeln(res, new iostyle(precision=10));
}
