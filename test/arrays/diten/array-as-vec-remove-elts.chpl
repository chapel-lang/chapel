var X: [1..10] int = 1..10;
config const remHighStart = X.domain.high-1;
config const remLowStart = X.domain.low;

writeln(X);

X.remove(remHighStart, 2);

writeln(X);

X.remove(remLowStart, 2);

writeln(X);
