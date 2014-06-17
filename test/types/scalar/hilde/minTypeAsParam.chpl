// The min and max type functions should return a param.
param minS = min(int(32));
param maxS = max(int(32));

writeln(minS);
writeln(maxS);

// I'd rather see these as properties on the type, thus:
param myMinS = int(32).min;
param myMaxS = int(32).max;

writeln(myMinS);
writeln(myMaxS);