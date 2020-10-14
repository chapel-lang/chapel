use BlockDist;

var A = newBlockArr({1..numLocales}, int);

on Locales[numLocales-1] {
  A.localAccess[numLocales] = numLocales;
}

// Loops should inhibit fast-on since the trip count can be arbitrarily large
on Locales[numLocales-1] {
  for i in numLocales..numLocales {
    A.localAccess[i] = i;
  }
}

writeln(A);
