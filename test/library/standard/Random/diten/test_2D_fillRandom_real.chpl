use Random;

config param useNPB = true;
config param rtype = if useNPB then RNG.NPB else RNG.PCG;

config const n = 10;
config const seed = 13;

proc main(){
  var x: [0..n-1, 0..n-1] real; 
  var y: [0..n**2 - 1] real;

  fillRandom(x, seed, algorithm=rtype);
  fillRandom(y, seed, algorithm=rtype);

  for (i, j) in x.domain do
    if x(i,j) != y(i * n + j) then
      writeln("error: x(", i, ",", j, ") != y(", i*n +j, ").");
}
