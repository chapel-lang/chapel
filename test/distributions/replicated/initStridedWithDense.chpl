use ReplicatedDist;

// Once upon a time this program would fail to compile due to an
// incorrectly-written 'dsiAssignDomain.'

var D : domain(1, strides=strideKind.any) dmapped Replicated() = {1..10};
writeln(D);
