/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org
   
   Original C contributed by Sebastien Loisel
   Conversion to Chapel by Albert Sidelnik
   Updated by Lydia Duncan
*/
use IO;

config const n = 500;

// Return: 1.0 / (i + j) * (i + j +1) / 2 + i + 1;
inline proc eval_A(i, j) : real
{
  //
  // 1.0 / (i + j) * (i + j +1) / 2 + i + 1;
  // n * (n+1) is an even number. Therefore, just (>> 1) for (/2)
  //
  const d = (((i + j) * (i + j + 1)) >> 1) + i + 1;
  return 1.0 / d;
}

//
// Traverses the array serially, performing the computation (eval_A)*U(k) on
// each pair i, k where k is a number from 0 to inRange - 1.  This method
// is generalized so that it can perform both versions of the call.
//
proc sum (i, inRange, param flip : bool, U : [] real) {
  var result = 0.0;
  for k in 0..#inRange do
    result += U(k) * ( if flip then eval_A(k, i) else eval_A(i, k) );
  return result;
}

//
// Fills the array Au with the result of calling sum with each index i from 0
// to inRange - 1, inRange, a boolean indicating not to flip the order for the
// inner call and the array U
//
proc eval_A_times_u(U : [] real, inRange, Au : [] real)
{
  forall i in 0..#inRange do
    Au(i) = sum(i, inRange, false, U);
}

//
// Fills the array Au with the result of calling sum with each index i from 0
// to inRange - 1, inRange, a boolean indicating to flip the order for the
// inner call and the array U
//
proc eval_At_times_u(U : [] real, inRange, Au : [] real)
{
  forall i in 0..#inRange do
    Au(i) = sum(i, inRange, true, U);
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
