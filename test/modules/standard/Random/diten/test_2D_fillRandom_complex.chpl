use Random;

config const n = 10;
config const seed = 12;

def main(){
  var x: [0..n-1, 0..n-1] complex; 
  var y: [0..n**2 - 1] complex;

  fillRandom(x, seed);
  fillRandom(y, seed);

  for (i, j) in x.domain do
    if x(i, j) != y(i * n + j) then
      writeln("error: x(", i, ",", j, ") != y(", i*n +j, ").");
}
