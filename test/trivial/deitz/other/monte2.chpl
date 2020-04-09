proc random(inout x: uint) {
  x = (x:int(64) * 16807 % 2147483647): uint;
  return x:real / 2147483647;
}

config var n   : uint = 100000,
           seed: uint = 31415926;

writeln((+ reduce for 1..n do (random(seed)**2 + random(seed)**2 < 1.0)) * 4 / n:real);
