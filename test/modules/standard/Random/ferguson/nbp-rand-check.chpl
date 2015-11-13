use Random;

var A:[1..10] real(64);

fillRandom(A, seed=1, algorithm=RNG.NPB);

var B:[1..10] int;
for (a,b) in zip(A,B) {
  b = (a * 2.0**46):int;
}

for b in B {
  writeln(b);
}

