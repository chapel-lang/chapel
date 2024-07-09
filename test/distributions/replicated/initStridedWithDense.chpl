use ReplicatedDist;

// Once upon a time this program would fail to compile due to an
// incorrectly-written 'dsiAssignDomain.'

var D : domain(1, strides=strideKind.any) dmapped new replicatedDist() = {1..10};
writeln(D);
