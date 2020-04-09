use Random;

config var n = 100;

config param useNPB = true;
config param rtype = if useNPB then RNG.NPB else RNG.PCG;

var randStr1 = createRandomStream(real, 314159265, algorithm=rtype);
var randStr2 = createRandomStream(real, 314159265, algorithm=rtype);
var randStr3 = createRandomStream(real, 314159265, algorithm=rtype);

var A, B: [1..n] real;

randStr1.fillRandom(A);

for i in 1..n {
  B(i) = randStr2.getNth(i-1);
}

checkArrays("1: ");

for i in 1..n by -1 {
  B(i) = randStr2.getNth(i-1);
}

checkArrays("2: ");

for i in 1..n by 2 {
  B(i) = randStr2.getNth(i-1);
  B(i+1) = randStr2.getNth(i);
}

checkArrays("3: ");

for i in 1..n by 4 {
  B(i) = randStr2.getNth(i-1);
  B(i+1) = randStr2.getNext();
  B(i+2) = randStr2.getNext();
  B(i+3) = randStr2.getNext();
}

checkArrays("4: ");

B(1) = randStr2.getNth(0);

for i in 2..n {
  B(i) = randStr2.getNext();
}

checkArrays("5: ");

randStr2.skipToNth(0);

for i in 1..n {
  B(i) = randStr2.getNext();
}

checkArrays("6: ");


randStr2.skipToNth(0);

randStr2.fillRandom(B);

checkArrays("7: ");


writeln("Done!");

proc checkArrays(str) {
  for i in 1..n {
    if (A(i) != B(i)) {
      writeln(str, "mismatch at #", i, ": ", A(i), " != ", B(i));
    }
  }
}
