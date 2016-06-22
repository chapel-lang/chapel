// questionable implementation because of multiple random streams

iter random(n: uint, seed: uint) {
  var x = seed;
  for 1..n {
    x = (x:int(64) * 16807 % 2147483647): uint;
    yield x:real / 2147483647;
  }
}

config const n: uint = 100000;

writeln((+ reduce (random(n,27182818)**2 + random(n,31415926)**2 < 1.0)) * 4 / n:real);
