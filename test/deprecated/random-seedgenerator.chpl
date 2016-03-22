use Random;

// Random.SeedGenerator is replaced by Random.SeedGenerators
// Should be removed in v1.14.

var x = SeedGenerator.currentTime;

// Old seed generator returned an odd time.
assert((x & 1) == 1);
